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
        m_size(0,0),
        m_isVisible(true),
        m_isEnabled(true),
        m_opacity(1.0f)
{
        notifyPositionUpdated();
        if (parent) m_manager = parent->manager();
}

void WidgetAbstract::setVisible(bool isVisible) {
        m_isVisible = isVisible;
}

bool WidgetAbstract::isVisible() const {
        return m_isVisible;
}

void WidgetAbstract::hide() {
        setVisible(false);
}

void WidgetAbstract::show() {
        setVisible(true);
}

void WidgetAbstract::setEnabled(bool isEnabled) {
        m_isEnabled = isEnabled;

        if (m_isEnabled) onWidgetEnable();
        else onWidgetDisable();
}

bool WidgetAbstract::isEnabled() const {
        return m_isEnabled;
}

void WidgetAbstract::disable() {
        setEnabled(false);
}

void WidgetAbstract::enable() {
        setEnabled(true);
}

void WidgetAbstract::setOpacity(float opacity) {
        m_opacity = glm::clamp(opacity, 0.0f, 1.0f);
        for (TreeNode* node: children()) ((WidgetAbstract*)node)->setOpacity(m_opacity);
}

float WidgetAbstract::opacity() const {
        return m_opacity;
}

void WidgetAbstract::setLeft(float x) {
        m_position.x = x;
        notifyPositionUpdated();
}

void WidgetAbstract::setTop(float y) {
        m_position.y = y;
        notifyPositionUpdated();
}

void WidgetAbstract::setPosition(const Vec2 &position) {
        m_position = position;
        notifyPositionUpdated();
}

float WidgetAbstract::left() const {
        return m_position.x;
}

float WidgetAbstract::top() const {
        return m_position.y;
}

Vec2 WidgetAbstract::position() const {
        return m_position;
}

void WidgetAbstract::alignHorizontal(WidgetHAlignment::Enum align) {
        IM_ASSERT(m_parent);

        switch(align) {
                case WidgetHAlignment::LEFT:
                        setLeft(0.0f);
                        break;
                case WidgetHAlignment::CENTER:
                        setLeft(((WidgetAbstract*)parent())->width()/2 - width()/2);
                        break;
                case WidgetHAlignment::RIGHT:
                        setLeft(((WidgetAbstract*)parent())->width() - width());
                        break;
        }
}

void WidgetAbstract::alignVertical(WidgetVAlignment::Enum align) {
        IM_ASSERT(m_parent);

        switch(align) {
                case WidgetVAlignment::TOP:
                        setTop(0.0f);
                        break;
                case WidgetVAlignment::CENTER:
                        setTop(((WidgetAbstract*)parent())->height()/2 - height()/2);
                        break;
                case WidgetVAlignment::BOTTOM:
                        setTop(((WidgetAbstract*)parent())->height() - height());
                        break;
        }
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

Vec2 WidgetAbstract::absoluteToLocal(const Vec2 &pos) {
        return pos - absolutePosition();
}

Vec2 WidgetAbstract::localToAbsolute(const Vec2 &pos) {
        return absolutePosition() + pos;
}

Vec2 WidgetAbstract::size() const {
        return m_size;
}

float WidgetAbstract::width() const {
        return m_size.x;
}

float WidgetAbstract::height() const {
        return m_size.y;
}

GuiManager* WidgetAbstract::manager() const {
        return m_manager;
}

void WidgetAbstract::processRender() {
        if (!m_isVisible) return;

        onRender();
}

bool WidgetAbstract::processMouseMove(int oldX, int oldY, int newX, int newY) {
        if (!m_isEnabled || !m_isVisible) return false;

        // Если мышь не была внутри и не внутри сейчас - не обрабатываем ее
        bool wasMouseInside = isInsideWidget(oldX, oldY);
        bool isMouseInside = isInsideWidget(newX, newY);
        if(!wasMouseInside && !isMouseInside) return false;

        /* Обрабатываем сначала детей
           Если дочерний виджет не среагировал - то обрабатываем данный
           Например, Label не должен реагировать на мышь, поэтому в перегруженных методах
           onMouseOver будет возвращаеться false, что даст false в processMouseMove. */
        for (TreeNode* node: children()) {
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

bool WidgetAbstract::processMousePress(int x, int y, char button) {
        if (!m_isEnabled || !m_isVisible) return false;

        // Если мышь не внутри - не обрабатываем нажатие
        if (!isInsideWidget(x, y)) return false;

        // Обрабатываем сначала детей
        for (TreeNode* node: children()) {
                bool done = ((WidgetAbstract*)node)->processMousePress(x, y, button);
                if (done) return true;
        }

        // Если ни в одном из дочерних виджетов не обработано - обрабатываем в данном
        bool result = onMousePress(x, y, button);
        return result;
}

bool WidgetAbstract::processMouseRelease(int x, int y, char button) {
        if (!m_isEnabled || !m_isVisible) return false;

        // Если мышь не внутри - не обрабатываем отжатие
        if (!isInsideWidget(x, y)) return false;

        // Обрабатываем детей
        for (TreeNode* node: children()) {
                bool done = ((WidgetAbstract*)node)->processMouseRelease(x, y, button);
                if (done) return true;
        }

        // Обрабатываем сам виджет
        bool result = onMouseRelease(x, y, button);
        return result;
}

void WidgetAbstract::processGlobalMouseMove(int x, int y) {
        if (!m_isEnabled || !m_isVisible) return;

        onGlobalMouseMove(x, y);
        for (TreeNode* node: children()) {
                ((WidgetAbstract*)node)->processGlobalMouseMove(x ,y);
        }
}

void WidgetAbstract::processGlobalMousePress(int x, int y, char button) {
        if (!m_isEnabled || !m_isVisible) return;

        onGlobalMousePress(x, y, button);
        for (TreeNode* node: children()) {
                ((WidgetAbstract*)node)->processGlobalMousePress(x ,y, button);
        }
}

void WidgetAbstract::processGlobalMouseRelease(int x, int y, char button) {
        if (!m_isEnabled || !m_isVisible) return;

        onGlobalMouseRelease(x, y, button);
        for (TreeNode* node: children()) {
                ((WidgetAbstract*)node)->processGlobalMouseRelease(x ,y, button);
        }
}

bool WidgetAbstract::isInsideWidget(int x, int y) {
        Vec2 min = absolutePosition();
        Vec2 max = min + size();
        return (x >= min.x && y >= min.y) && (x <= max.x && y <= max.y);
}

void WidgetAbstract::renderChildren() {
        for (TreeNode* node: children()) ((WidgetAbstract*)node)->processRender();
}

void WidgetAbstract::onAttachChild(TreeNode *node) {
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
