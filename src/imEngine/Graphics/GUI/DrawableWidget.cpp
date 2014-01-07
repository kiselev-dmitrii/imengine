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


HStretchableWidget::HStretchableWidget(const String &initalImage, Widget *parent) :
        DrawableWidget(initalImage, parent)
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
        IM_VAR(width);

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

} //namespace imEngine
