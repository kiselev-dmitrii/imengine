#include "TexturedWidget.h"
#include <imEngine/Utils/Debug.h>

namespace imEngine {

/// Описывает элемент виджета. Данная структура передается в шейдер
struct WidgetElementGeometry {
        Vec2    offset;         // Относительная позция элемента
        Vec2    size;           // Размер элемента
        Vec4    texCoords;      // Текстурные коориданты в атласе
};

TexturedWidgetAbstract::TexturedWidgetAbstract(const String &initalImage, WidgetAbstract *parent) :
        WidgetAbstract(parent),
        m_isNeedToUpdateBuffer(true),
        m_widgetElementCount(0),
        m_currentImage(initalImage)
{
        setCurrentImage(initalImage);
        initBuffer();
}

void TexturedWidgetAbstract::onRender() {
        if (m_isNeedToUpdateBuffer) {
                updateBuffer();
                m_isNeedToUpdateBuffer = false;
        }

        // Биндим программу и рисуем
        manager()->program()->bind();
        manager()->textureAtlas()->bind(0);                                     // нужно ребиндить текстуру - так как текстурные блоки не меняются вместе с программами
        manager()->program()->setUniform("u_position", absolutePosition());
        manager()->program()->setUniform("u_opacity", opacity());
        m_vao->bind();
        Renderer::renderVertices(Primitive::POINT, m_widgetElementCount);

        // Рендерим детей виджета
        renderChildren();
}

void TexturedWidgetAbstract::initBuffer() {
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

void TexturedWidgetAbstract::setCurrentImage(const String &imageName) {
        if (imageName == m_currentImage) return;
        m_currentImage = imageName;
        m_isNeedToUpdateBuffer = true;
}

String TexturedWidgetAbstract::currentImage() const {
        return m_currentImage;
}

void TexturedWidgetAbstract::setWidgetElementCount(uint count) {
        m_widgetElementCount = count;
}


//######################## NonStretchableWidget #############################//


NonStretchableTexturedWidget::NonStretchableTexturedWidget(const String &initialImage, WidgetAbstract *parent) :
        TexturedWidgetAbstract(initialImage, parent)
{
        m_size = manager()->imageGeometry(currentImage())->size;
        setWidgetElementCount(1);
}

void NonStretchableTexturedWidget::updateBuffer() {
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


HStretchableTexturedWidget::HStretchableTexturedWidget(const String &initialImage, WidgetAbstract *parent) :
        TexturedWidgetAbstract(initialImage, parent),
        HStretchableAbstract()
{
        m_size = manager()->imageGeometry(currentImage())->size;
        setMinimalWidth(2 * m_size.x / 3);
        setWidgetElementCount(3);
}

void HStretchableTexturedWidget::setWidth(float width) {
        if (width == m_size.x) return;
        if (width < minimalWidth()) return;

        m_size.x = width;
        onWidthChange();

        m_isNeedToUpdateBuffer = true;
}

void HStretchableTexturedWidget::updateBuffer() {
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


VStretchableTexturedWidget::VStretchableTexturedWidget(const String &initialImage, WidgetAbstract *parent) :
        TexturedWidgetAbstract(initialImage, parent),
        VStretchableAbstract()
{
        m_size = manager()->imageGeometry(currentImage())->size;
        setMinimalHeight(2 * m_size.y / 3);
        setWidgetElementCount(3);
}

void VStretchableTexturedWidget::setHeight(float height) {
        if (height == m_size.y) return;
        if (height < minimalHeight()) return;

        m_size.y = height;
        onHeightChange();

        m_isNeedToUpdateBuffer = true;
}

void VStretchableTexturedWidget::updateBuffer() {
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


BothStretchableTexturedWidget::BothStretchableTexturedWidget(const String &initialImage, WidgetAbstract *parent) :
        TexturedWidgetAbstract(initialImage, parent),
        BothStretchableAbstract()
{
        m_size = manager()->imageGeometry(currentImage())->size;
        setMinimalSize(Vec2(2*m_size.x/3, 2*m_size.y/3));
        setWidgetElementCount(9);
}

void BothStretchableTexturedWidget::setWidth(float width) {
        if (width == m_size.x) return;
        if (width < minimalSize().x) return;

        m_size.x = width;
        onWidthChange();

        m_isNeedToUpdateBuffer = true;
}

void BothStretchableTexturedWidget::setHeight(float height) {
        if (height == m_size.y) return;
        if (height < minimalSize().y) return;

        m_size.y = height;
        onHeightChange();

        m_isNeedToUpdateBuffer = true;
}

void BothStretchableTexturedWidget::updateBuffer() {
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
