#include "GuiManager.h"
#include <imEngine/System/Filesystem.h>
#include <imEngine/FileContainers/Image.h>
#include "Widget.h"
#include "WidgetProgram.glsl"

namespace imEngine {

GuiManager::GuiManager(const String &themePath) {
        setTheme(themePath);
        m_program = createProgram();

        class EmptyWidget : public Widget {
        public:
                void render() { renderChilds(); }
        };
        m_rootWidget = new EmptyWidget();
}

GuiManager::~GuiManager() {
        delete m_rootWidget;
}

void GuiManager::setTheme(const String &themePath) {
        if (themePath == m_themePath) return;
        m_themePath = themePath;

        StringList files = Filesystem::tree(m_themePath);
        ImageList images;
        for (String& file: files) images.push_back(ImagePtr(new Image(file)));

        m_texture = generateTextureAtlas(images);
        fillImagesGeometry(images, files, m_texture, &m_imagesGeometry);
}

String GuiManager::themePath() {
        return m_themePath;
}

Texture2DPtr GuiManager::textureAtlas() const {
        return m_texture;
}

void GuiManager::attachWidget(Widget *widget) {
        m_rootWidget->attachChild(widget);
}

void GuiManager::detachWidget(Widget *widget) {
        m_rootWidget->detachChild(widget);
}

ImageGeometry* GuiManager::imageGeometry(const String &name) {
        auto it = m_imagesGeometry.find(name);
        if (it != m_imagesGeometry.end()) return &((*it).second);
        return nullptr;
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
        TextureSrcType::Enum srcType = images[0]->type();                // формат и тип у всех изображений один и тот же
        TextureSrcFormat::Enum srcFormat = images[0]->format();
        texture->allocate(size.x, size.y, TextureInternalFormat::COLOR_FLOAT_4_COMP_32_BIT, srcType, srcFormat);
        texture->clear();

        /// Вставляем наши изображения в текстуру
        uint startX = 0;
        for (ImagePtr image: images) {
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
