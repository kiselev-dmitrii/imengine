#include "Widget.h"
#include <imEngine/Utils/Debug.h>

namespace imEngine {

//################## Widget #######################//

Widget::Widget(Widget *parent) :
        TreeNode(parent),
        m_manager(nullptr),
        m_position(0, 0),
        m_absolutePosition(0,0),
        m_isNeedToUpdateAbsolutePosition(false),
        m_size(0,0)
{
        notifyPositionUpdated();
}

void Widget::initialize(GuiManager *manager) {
        m_manager = manager;
}

void Widget::setPosition(const Vec2 &position) {
        m_position = position;
        notifyPositionUpdated();
}

Vec2 Widget::position() const {
        return m_position;
}

Vec2 Widget::absolutePosition() {
        updateAbsolutePosition();
        return m_absolutePosition;
}

void Widget::setAbsolutePosition(const Vec2 &position) {
        if (m_parent) {
                setPosition(position - ((Widget*)m_parent)->absolutePosition());
        } else {
                setPosition(position);
        }
}

Vec2 Widget::size() const {
        return m_size;
}

GuiManager* Widget::manager() const {
        return m_manager;
}

void Widget::onAttachChild(TreeNode *node) {
        ((Widget*)node)->initialize(m_manager);
        notifyPositionUpdated();
}

void Widget::onDetachChild(TreeNode *node) {
        notifyPositionUpdated();
}

void Widget::notifyPositionUpdated() {
        m_isNeedToUpdateAbsolutePosition = true;
        for (TreeNode* node: children()) {
                ((Widget*)node)->notifyPositionUpdated();
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


} //namespace imEngine
