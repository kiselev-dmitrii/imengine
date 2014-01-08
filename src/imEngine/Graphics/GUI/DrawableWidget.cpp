#include "DrawableWidget.h"
#include <imEngine/Utils/Debug.h>

namespace imEngine {

/// Описывает элемент виджета. Данная структура передается в шейдер
struct WidgetElementGeometry {
        Vec2    offset;         // Относительная позция элемента
        Vec2    size;           // Размер элемента
        Vec4    texCoords;      // Текстурные коориданты в атласе
};

DrawableWidget::DrawableWidget(const String &initalImage, Widget *parent) :
        Widget(parent),
        m_isNeedToUpdateBuffer(true),
        m_widgetElementCount(0),
        m_currentImage(initalImage)
{
        setCurrentImage(initalImage);
}

void DrawableWidget::initialize(GuiManager *manager) {
        Widget::initialize(manager);
        initBuffer();
}

void DrawableWidget::render() {
        if (m_isNeedToUpdateBuffer) {
                updateBuffer();
                m_isNeedToUpdateBuffer = false;
        }

        // Биндим программу и рисуем
        manager()->program()->bind();
        manager()->program()->setUniform("u_position", absolutePosition());
        m_vao->bind();
        Renderer::renderVertices(Primitive::POINT, m_widgetElementCount);

        // Отрисовываем детишек
        for (TreeNode* node: children()) ((Widget*)node)->render();
}

void DrawableWidget::initBuffer() {
        IM_ASSERT(manager());

        m_vbo = VertexBufferPtr(new VertexBuffer());
        m_vao = VertexArrayPtr(new VertexArray());

        m_vao->bind();
                m_vbo->connect(manager()->program()->attributeLocation("in_offset"), 2, GL_FLOAT,
                       offsetof(WidgetElementGeometry, offset), sizeof(WidgetElementGeometry));
                m_vbo->connect(manager()->program()->attributeLocation("in_size"), 2, GL_FLOAT,
                       offsetof(WidgetElementGeometry, size) , sizeof(WidgetElementGeometry));
                m_vbo->connect(manager()->program()->attributeLocation("in_texCoords"), 4, GL_FLOAT,
                       offsetof(WidgetElementGeometry, texCoords), sizeof(WidgetElementGeometry));
        m_vao->unbind();
}

void DrawableWidget::setCurrentImage(const String &imageName) {
        if (imageName == m_currentImage) return;
        m_currentImage = imageName;
        m_isNeedToUpdateBuffer = true;
}

String DrawableWidget::currentImage() const {
        return m_currentImage;
}

void DrawableWidget::setWidgetElementCount(uint count) {
        m_widgetElementCount = count;
}


//######################## NonStretchableWidget #############################//


NonStretchableWidget::NonStretchableWidget(const String &initialImage, Widget *parent) :
        DrawableWidget(initialImage, parent)
{ }

void NonStretchableWidget::initialize(GuiManager *manager) {
        DrawableWidget::initialize(manager);
        m_size = manager->imageGeometry(currentImage())->size;
        setWidgetElementCount(1);
}

void NonStretchableWidget::updateBuffer() {
        /// Получаем информацию о изображении в текстурном атласе
        ImageGeometry* g = manager()->imageGeometry(currentImage());
        if (!g) {
                IM_ERROR("Image " << currentImage() << " not found");
                return;
        }

        // Заполняем данные и грузим их в VBO
        WidgetElementGeometry data;
        data.offset = Vec2(0,0);
        data.size = g->size;
        data.texCoords = g->texCoords;
        m_vbo->load(&data, sizeof(data), BufferUsage::STREAM_DRAW);
}


//######################## HStretchableWidget #############################//


HStretchableWidget::HStretchableWidget(const String &initialImage, Widget *parent) :
        DrawableWidget(initialImage, parent)
{ }

void HStretchableWidget::initialize(GuiManager *manager) {
        DrawableWidget::initialize(manager);
        m_size = manager->imageGeometry(currentImage())->size;
        m_minimalWidth = 2 * m_size.x / 3;
        setWidgetElementCount(3);
}

void HStretchableWidget::setWidth(float width) {
        if (width == m_size.x) return;
        if (width < m_minimalWidth) return;

        m_size.x = width;
        m_isNeedToUpdateBuffer = true;
}

void HStretchableWidget::setMinimalWidth(float width) {
        m_minimalWidth = width;
}

float HStretchableWidget::minimalWidth() const {
        return m_minimalWidth;
}

void HStretchableWidget::updateBuffer() {
        /// Получаем информацию о изображении в текстурном атласе
        ImageGeometry* g = manager()->imageGeometry(currentImage());
        if (!g) {
                IM_ERROR("Image " << currentImage() << " not found");
                return;
        }
        float wgtWidth = size().x;
        float wgtHeight = size().y;
        float imgWidth = g->size.x;
        float sideWidth = imgWidth/3;

        float texCoordWidth = g->texCoords.z - g->texCoords.x;

        // Заполняем данные и грузим их в VBO
        WidgetElementGeometry data[3];

        // Осторожно, write-only код
        // Первый элемент - начало виджета (кнопки или еще чего)
        data[0].offset = Vec2(0,0);
        data[0].size = Vec2(sideWidth, wgtHeight);
        data[0].texCoords.x = g->texCoords.x;
        data[0].texCoords.y = g->texCoords.y;
        data[0].texCoords.z = g->texCoords.x + texCoordWidth/3;
        data[0].texCoords.w = g->texCoords.w;

        // Второй элемент - который растягивается или тайлится
        data[1].offset = Vec2(sideWidth, 0);
        data[1].size = Vec2(wgtWidth - 2*sideWidth, wgtHeight);
        data[1].texCoords.x = g->texCoords.x + texCoordWidth/3;
        data[1].texCoords.y = g->texCoords.y;
        data[1].texCoords.z = g->texCoords.x + 2*texCoordWidth/3;
        data[1].texCoords.w = g->texCoords.w;

        // Последний элемент - конец виджета
        data[2].offset = Vec2(data[0].size.x + data[1].size.x, 0);
        data[2].size = Vec2(sideWidth, wgtHeight);
        data[2].texCoords.x = g->texCoords.x + 2*texCoordWidth/3;
        data[2].texCoords.y = g->texCoords.y;
        data[2].texCoords.z = g->texCoords.z;
        data[2].texCoords.w = g->texCoords.w;

        // Грузим данные в VBO
        m_vbo->load(&data, sizeof(data), BufferUsage::STREAM_DRAW);
}


//######################## VStretchableWidget #############################//


VStretchableWidget::VStretchableWidget(const String &initialImage, Widget *parent) :
        DrawableWidget(initialImage, parent)
{ }


void VStretchableWidget::initialize(GuiManager *manager) {
        DrawableWidget::initialize(manager);
        m_size = manager->imageGeometry(currentImage())->size;
        m_minimalHeight = 2 * m_size.y / 3;
        setWidgetElementCount(3);
}

void VStretchableWidget::setHeight(float height) {
        if (height == m_size.y) return;
        if (height < m_minimalHeight) return;

        m_size.y = height;
        m_isNeedToUpdateBuffer = true;
}

void VStretchableWidget::setMinimalHeight(float height) {
        m_minimalHeight = height;
}

float VStretchableWidget::minimalHeight() const {
        return m_minimalHeight;
}

void VStretchableWidget::updateBuffer() {
        ImageGeometry* g = manager()->imageGeometry(currentImage());
        if (!g) {
                IM_ERROR("Image " << currentImage() << " not found");
                return;
        }

        float wgtHeight = size().y;
        float imgWidth = g->size.x;
        float imgHeight = g->size.y;
        float sideHeight = imgHeight/3;
        float tcDiff = g->texCoords.w - g->texCoords.y;

        // Заполняем массив данными
        WidgetElementGeometry data[3];

        // Первый верхний элемент
        data[0].offset = Vec2(0,0);
        data[0].size = Vec2(imgWidth, sideHeight);
        data[0].texCoords.x = g->texCoords.x;
        data[0].texCoords.y = g->texCoords.y + 2*tcDiff/3;
        data[0].texCoords.z = g->texCoords.z;
        data[0].texCoords.w = g->texCoords.w;

        // Середина - которая растягивается по высоте
        data[1].offset = Vec2(0, sideHeight);
        data[1].size = Vec2(imgWidth, wgtHeight - 2*sideHeight);
        data[1].texCoords.x = g->texCoords.x;
        data[1].texCoords.y = g->texCoords.y + tcDiff/3;
        data[1].texCoords.z = g->texCoords.z;
        data[1].texCoords.w = g->texCoords.y + 2*tcDiff/3;

        // Последний элемент
        data[2].offset = Vec2(0, data[0].size.y + data[1].size.y);
        data[2].size = Vec2(imgWidth, sideHeight);
        data[2].texCoords.x = g->texCoords.x;
        data[2].texCoords.y = g->texCoords.y;
        data[2].texCoords.z = g->texCoords.z;
        data[2].texCoords.w = g->texCoords.y + tcDiff/3;

        // Грузим данные в VBO
        m_vbo->load(&data, sizeof(data), BufferUsage::STREAM_DRAW);
}


//######################## BothStretchableWidget #############################//


BothStretchableWidget::BothStretchableWidget(const String &initialImage, Widget *parent) :
        DrawableWidget(initialImage, parent)
{ }

void BothStretchableWidget::initialize(GuiManager *manager) {
        DrawableWidget::initialize(manager);
        m_size = manager->imageGeometry(currentImage())->size;
        m_minimalSize = Vec2(2*m_size.x/3, 2*m_size.y/3);
        setWidgetElementCount(9);
}

void BothStretchableWidget::setSize(const Vec2 &size) {
        if (size == m_size) return;
        if (size.x < m_minimalSize.x || size.y < m_minimalSize.y) {
                return;
        }

        m_size = size;
        m_isNeedToUpdateBuffer = true;
}

void BothStretchableWidget::setMinimalSize(const Vec2 &size) {
        m_minimalSize = size;
}

Vec2 BothStretchableWidget::minimalSize() const {
        return m_minimalSize;
}

void BothStretchableWidget::updateBuffer() {
        ImageGeometry* g = manager()->imageGeometry(currentImage());
        if (!g) {
                IM_ERROR("Image " << currentImage() << " not found");
                return;
        }

        float wgtWidth = size().x;
        float wgtHeight = size().y;
        float sideWidth = g->size.x/3;
        float sideHeight = g->size.y/3;
        float middleWidth = wgtWidth - 2 * sideWidth;
        float middleHeight = wgtHeight - 2 * sideHeight;

        float tcDX = (g->texCoords.z - g->texCoords.x)/3;
        float tcDY = (g->texCoords.w - g->texCoords.y)/3;
        float tcX = g->texCoords.x;
        float tcY = g->texCoords.y;

        // Заполняем массив данными построчно
        WidgetElementGeometry data[9];

        // Левый верхний угол
        data[0].offset = Vec2(0,0);
        data[0].size = Vec2(sideWidth, sideHeight);
        data[0].texCoords = Vec4(tcX, tcY+2*tcDY, tcX+tcDX, tcY+3*tcDY);

        // Верхняя середина
        data[1].offset = Vec2(sideWidth, 0);
        data[1].size = Vec2(middleWidth, sideHeight);
        data[1].texCoords = Vec4(tcX+tcDX, tcY+2*tcDY, tcX+2*tcDX, tcY+3*tcDY);

        // Правый верхний угол
        data[2].offset = Vec2(sideWidth + middleWidth, 0);
        data[2].size = Vec2(sideWidth, sideHeight);
        data[2].texCoords = Vec4(tcX+2*tcDX, tcY+2*tcDY, tcX+3*tcDX, tcY+3*tcDY);

        // Левая средняя сторона
        data[3].offset = Vec2(0, sideHeight);
        data[3].size = Vec2(sideWidth, middleHeight);
        data[3].texCoords = Vec4(tcX, tcY+tcDY, tcX+tcDX, tcY+2*tcDY);

        // Середина
        data[4].offset = Vec2(sideWidth, sideHeight);
        data[4].size = Vec2(middleWidth, middleHeight);
        data[4].texCoords = Vec4(tcX+tcDX, tcY+tcDY, tcX+2*tcDX, tcY+2*tcDY);

        // Правая средняя сторона
        data[5].offset = Vec2(sideWidth + middleWidth, sideHeight);
        data[5].size = Vec2(sideWidth, middleHeight);
        data[5].texCoords = Vec4(tcX+2*tcDX, tcY+tcDY, tcX+3*tcDX, tcY+2*tcDY);

        // Нижняя левая сторона
        data[6].offset = Vec2(0, sideHeight + middleHeight);
        data[6].size = Vec2(sideWidth, sideHeight);
        data[6].texCoords = Vec4(tcX, tcY, tcX+tcDX, tcY+tcDY);

        // Нижняя средняя сторона
        data[7].offset = Vec2(sideWidth, sideHeight + middleHeight);
        data[7].size = Vec2(middleWidth, sideHeight);
        data[7].texCoords = Vec4(tcX+tcDX, tcY, tcX+2*tcDX, tcY+tcDY);

        // Нижняя правая сторона
        data[8].offset = Vec2(sideWidth + middleWidth, sideHeight +  middleHeight);
        data[8].size = Vec2(sideWidth, sideHeight);
        data[8].texCoords = Vec4(tcX+2*tcDX, tcY, tcX+3*tcDX, tcY+tcDY);

        // Грузим данные в VBO
        m_vbo->load(&data, sizeof(data), BufferUsage::STREAM_DRAW);
}

} //namespace imEngine
