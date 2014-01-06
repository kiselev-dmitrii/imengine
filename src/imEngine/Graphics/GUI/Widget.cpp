#include "Widget.h"
#include <imEngine/Utils/Debug.h>

namespace imEngine {

/// Описывает элемент виджета. Данная структура передается в шейдер
struct WidgetElementGeometry {
        Vec2    offset;         // Относительная позция элемента
        Vec2    size;           // Размер элемента
        Vec4    texCoords;      // Текстурные коориданты в атласе
};

//################## Widget #######################//

Widget::Widget(Widget *parent) :
        TreeNode(parent),
        m_manager(nullptr),
        m_position(0, 0),
        m_absolutePosition(0,0),
        m_isNeedToUpdateAbsolutePosition(false),
        m_size(0,0)
{
        notifyChildren();
}

void Widget::setPosition(const Vec2 &position) {
        m_position = position;
        notifyChildren();
}

Vec2 Widget::position() const {
        return m_position;
}

Vec2 Widget::absolutePosition() {
        updateAbsolutePosition();
        return m_absolutePosition;
}

Vec2 Widget::size() const {
        return m_size;
}

GuiManager* Widget::manager() const {
        return m_manager;
}

void Widget::setCurrentImage(const String &name) {
        m_currentImage = name;
        m_isNeedToUpdateBuffer = true;
}

String Widget::currentImage() const {
        return m_currentImage;
}

void Widget::onAttachChild(TreeNode *node) {
        ((Widget*)node)->m_manager = m_manager;
        notifyChildren();
}

void Widget::onDetachChild(TreeNode *node) {
        ((Widget*)node)->m_manager = nullptr;
        notifyChildren();
}

void Widget::renderAllChildren() {
        for (TreeNode* child: m_children) {
                ((Widget*)child)->render();
        }
}

void Widget::notifyChildren() {
        m_isNeedToUpdateAbsolutePosition = true;
        for (TreeNode* node: children()) {
                ((Widget*)node)->notifyChildren();
        }
}

void Widget::updateAbsolutePosition() {
        if (m_isNeedToUpdateAbsolutePosition) {
                if (m_parent) {
                        m_absolutePosition = ((Widget*)m_parent)->absolutePosition() + m_position;
                } else {
                        m_absolutePosition = m_position;
                }

                m_isNeedToUpdateAbsolutePosition = false;
        }
}

void Widget::initBuffer() {
        IM_ASSERT(m_manager);

        m_vbo = VertexBufferPtr(new VertexBuffer());
        m_vao = VertexArrayPtr(new VertexArray());

        m_vao->bind();
                m_vbo->connect(m_manager->program()->attributeLocation("in_offset"), 2, GL_FLOAT,
                       offsetof(WidgetElementGeometry, offset), sizeof(WidgetElementGeometry));
                m_vbo->connect(m_manager->program()->attributeLocation("in_size"), 2, GL_FLOAT,
                       offsetof(WidgetElementGeometry, size) , sizeof(WidgetElementGeometry));
                m_vbo->connect(m_manager->program()->attributeLocation("in_texCoords"), 4, GL_FLOAT,
                       offsetof(WidgetElementGeometry, texCoords), sizeof(WidgetElementGeometry));
        m_vao->unbind();
}

//######################### NonStrachableWidget ###################//

NonStretchableWidget::NonStretchableWidget(const String &initImage, Widget *parent) : Widget(parent) {
        IM_TRACE("Это хак, и не будет работать, если parent=nullptr");
        m_manager = parent->manager();
        initBuffer();
        setCurrentImage(initImage);
}

void NonStretchableWidget::render() {
        if (m_isNeedToUpdateBuffer) {
                updateBuffer();
                m_isNeedToUpdateBuffer = false;
        }

        manager()->program()->bind();

        manager()->textureAtlas()->bind(0);
        manager()->program()->setUniform("u_texture", 0);
        manager()->program()->setUniform("u_windowSize", Vec2(manager()->window()->size()));
        manager()->program()->setUniform("u_position", absolutePosition());

        // Биндим буфер и рисуем
        m_vao->bind();
        Renderer::renderVertices(Primitive::POINT, 1);
}

void NonStretchableWidget::updateBuffer() {
        /// Получаем информацию о изображении в текстурном атласе
        ImageGeometry* g = manager()->imageGeometry(currentImage());
        if (!g) return;

        // Заполняем данные и грузим их в VBO
        WidgetElementGeometry data;
        data.offset = Vec2(0,0);
        data.size = g->size;
        data.texCoords = g->texCoords;
        m_vbo->load(&data, sizeof(data), BufferUsage::STREAM_DRAW);

        // Обновляем размер виджета
        m_size = g->size;
}

} //namespace imEngine
