#include "GuiManager.h"
#include <imEngine/System/Filesystem.h>
#include <imEngine/FileContainers/Image.h>
#include <imEngine/Utils/Debug.h>
#include <imEngine/Application/GraphicApplication.h>
#include <limits>
#include "Widget.h"
#include "WidgetProgram.glsl"

namespace imEngine {

/** @brief Пустой виджет, служащий в качестве контейнера для других.
 *  Имеет максимально возможный размер, чтобы вмещать в себе остальные виджеты
 */
class RootWidget : public WidgetAbstract {
public:
        /// Конструктор. Устанавливаем максимально возможный размер, чтобы обрабатывались нажатия для всех детей
        RootWidget(GuiManager* manager) : WidgetAbstract(nullptr) {
                m_manager = manager;
                m_size = Vec2(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
        }
};


//############################# GuiManager ###################################//


GuiManager::GuiManager(const String &themePath, GraphicApplication *application) :
        m_application(application)
{
        setTheme(themePath);
        m_program = createProgram();
        m_root = new RootWidget(this);
}

GuiManager::~GuiManager() {
        delete m_root;
}

void GuiManager::setTheme(const String &themePath) {
        if (themePath == m_themePath) return;
        m_themePath = themePath;

        // Получаем список путей и грузим по ним изображения
        StringList files = Filesystem::tree(m_themePath);
        ImageList images;
        for (String& file: files) images.push_back(ImagePtr(new Image(file)));

        // Использовать полный путь - отстой, поэтому убираем из путей имя директории
        for (String& file: files) file.replace(0, themePath.length(), "");

        // Генерируем текстурку и вспомогательную информацию
        m_texture = generateTextureAtlas(images);
        fillImagesGeometry(images, files, m_texture, &m_imagesGeometry);
}

String GuiManager::themePath() {
        return m_themePath;
}

GraphicApplication* GuiManager::application() const {
        return m_application;
}

Window* GuiManager::window() const {
        return m_application->mainWindow();
}

Texture2DPtr GuiManager::textureAtlas() const {
        return m_texture;
}

ProgramPtr GuiManager::program() const {
        return m_program;
}

ImageGeometry* GuiManager::imageGeometry(const String &name) {
        auto it = m_imagesGeometry.find(name);
        if (it != m_imagesGeometry.end()) return &((*it).second);
        return nullptr;
}

StringList GuiManager::imageList() const {
        StringList result;
        for (auto& pair: m_imagesGeometry) result.push_back(pair.first);
        return result;
}

WidgetAbstract* GuiManager::root() const {
        return m_root;
}

void GuiManager::processRender() {
        Renderer::setDepthMode(DepthMode::NONE);
        Renderer::setBlendMode(BlendMode::ALPHA);

        m_program->bind();
        m_texture->bind(0);
        m_program->setUniform("u_texture", 0);
        m_program->setUniform("u_windowSize", Vec2(window()->size()));
        m_root->processRender();

}

void GuiManager::processUpdate() {
        m_root->processUpdate();
}

void GuiManager::processMouseMove(int oldX, int oldY, int newX, int newY) {
        m_root->processMouseMove(oldX, oldY, newX, newY);
        m_root->processGlobalMouseMove(newX, newY);
}

void GuiManager::processMousePress(int x, int y, char button) {
        m_root->processMousePress(x, y, button);
        m_root->processGlobalMousePress(x, y, button);
}

void GuiManager::processMouseRelease(int x, int y, char button) {
        m_root->processMouseRelease(x, y, button);
        m_root->processGlobalMouseRelease(x, y, button);
}

void GuiManager::processKeyPress(int key) {
        WidgetAbstract* wgt = WidgetAbstract::focusedWidget();
        if (wgt) wgt->onKeyPress(key);
}

void GuiManager::processKeyRelease(int key) {
        WidgetAbstract* wgt = WidgetAbstract::focusedWidget();
        if (wgt) wgt->onKeyRelease(key);
}

IVec2 GuiManager::calcSizeOfTextureAtlas(const ImageList &images) const {
        IVec2 size(0,0);

        // Все изображения будут расставлены в одну строчку
        for (ImagePtr image: images) {
                size.x += image->width();
                size.y = std::max(image->height(), size.y);
        }
        return size;
}

Texture2DPtr GuiManager::generateTextureAtlas(const ImageList &images) const {
        Texture2DPtr texture(new Texture2D());
        texture->setMagnificationFilter(TextureMagFilter::LINEAR);
        texture->setMinimizationFilter(TextureMinFilter::LINEAR);

        IVec2 size = calcSizeOfTextureAtlas(images);
        SourceType::Enum srcType = images[0]->type();                // формат и тип у всех изображений один и тот же
        SourceFormat::Enum srcFormat = images[0]->format();
        texture->allocate(size.x, size.y, InternalFormat::COLOR_FLOAT_4_COMP_32_BIT, srcType, srcFormat);
        texture->clear();

        /// Вставляем наши изображения в текстуру
        uint startX = 0;
        for (ImagePtr image: images) {
                image->flipVertically();      // OpenGL должен получать данные изображения в отраженном виде
                texture->insert(startX, 0, image->width(), image->height(), image->data());
                startX += image->width();
        }

        return texture;

}

ProgramPtr GuiManager::createProgram() const {
        ProgramPtr program(new Program());
        program->loadSource(widgetProgramSource);
        program->build();

        return program;
}

void GuiManager::fillImagesGeometry(const ImageList &images, const StringList &files, Texture2DPtr texture, MapImageGeometry *out) const {
        float startX = 0;
        for (uint i = 0; i < images.size(); ++i) {
                ImageGeometry g;

                g.size = Vec2(images[i]->width(), images[i]->height());
                g.texCoords.x = startX/texture->width();
                g.texCoords.y = 0;
                g.texCoords.z = (startX + g.size.x)/texture->width();
                g.texCoords.w = g.size.y/texture->height();

                out->insert(std::pair<String, ImageGeometry>(files[i], g));

                startX += g.size.x;
        }
}

} //namespace imEngine
