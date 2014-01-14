#include "Widget.h"
#include <imEngine/Utils/Debug.h>

namespace imEngine {

//################## Widget #######################//

WidgetAbstract::WidgetAbstract(WidgetAbstract *parent) :
        TreeNode(parent),
        m_manager(nullptr),
        m_position(0, 0),
        m_absolutePosition(0,0),
        m_isNeedToUpdateAbsolutePosition(false),
        m_size(0,0)
{
        notifyPositionUpdated();
}

void WidgetAbstract::initialize(GuiManager *manager) {
        IM_ASSERT(manager);
        m_manager = manager;
}

bool WidgetAbstract::processMouseMove(int oldX, int oldY, int newX, int newY) {
        // Если мышь не была внутри или не внутри сейчас - не обрабатываем ее
        bool wasMouseInside = isInsideWidget(oldX, oldY);
        bool isMouseInside = isInsideWidget(newX, newY);
        if(!wasMouseInside || !isMouseInside) return false;

        for(TreeNode* node: children()) {
                /* Обрабатываем сначала детей
                   Если дочерний виджет не среагировал - то обрабатываем данный
                   Например, Label не должен реагировать на мышь, поэтому в перегруженных методах
                   onMouseOver будет возвращаеться false, что даст false в processMouseMove. */
                bool done = ((WidgetAbstract*)node)->processMouseMove(oldX, oldY, newX, newY);
                if (done) return true;
        }

        /* Если никакой из дочерних виджетов (вообще никакой по иерархии),
           то обрабатываем данный виджет */
        bool result = false;
        result = result || onMouseMove(newX, newY);
        if (wasMouseInside && !isMouseInside) result = result || onMouseLeave(newX, newY);
        if (!wasMouseInside && isMouseInside) result = result || onMouseEnter(newX, newY);

        return result;
}

void WidgetAbstract::setPosition(const Vec2 &position) {
        m_position = position;
        notifyPositionUpdated();
}

Vec2 WidgetAbstract::position() const {
        return m_position;
}

Vec2 WidgetAbstract::absolutePosition() {
        updateAbsolutePosition();
        return m_absolutePosition;
}

void WidgetAbstract::setAbsolutePosition(const Vec2 &position) {
        if (m_parent) {
                setPosition(position - ((WidgetAbstract*)m_parent)->absolutePosition());
        } else {
                setPosition(position);
        }
}

Vec2 WidgetAbstract::size() const {
        return m_size;
}

GuiManager* WidgetAbstract::manager() const {
        return m_manager;
}

void WidgetAbstract::renderChildren() {
        for (TreeNode* node: children()) ((WidgetAbstract*)node)->render();
}

bool WidgetAbstract::isInsideWidget(int x, int y) {
        Vec2 min = absolutePosition();
        Vec2 max = min + size();
        return (x >= min.x && y >= min.y) && (x <= max.x && y <= max.y);
}

void WidgetAbstract::onAttachChild(TreeNode *node) {
        ((WidgetAbstract*)node)->initialize(m_manager);
        notifyPositionUpdated();
}

void WidgetAbstract::onDetachChild(TreeNode *node) {
        notifyPositionUpdated();
}

void WidgetAbstract::notifyPositionUpdated() {
        m_isNeedToUpdateAbsolutePosition = true;
        for (TreeNode* node: children()) {
                ((WidgetAbstract*)node)->notifyPositionUpdated();
        }
}

void WidgetAbstract::updateAbsolutePosition() {
        if (m_isNeedToUpdateAbsolutePosition) {
                if (m_parent) {
                        m_absolutePosition = ((WidgetAbstract*)m_parent)->absolutePosition() + m_position;
                } else {
                        m_absolutePosition = m_position;
                }

                m_isNeedToUpdateAbsolutePosition = false;
        }
}


} //namespace imEngine
