#include "Widget.h"
#include <imEngine/Utils/Debug.h>

namespace imEngine {

//################## Widget #######################//

WidgetAbstract* WidgetAbstract::s_focusedWidget = nullptr;

WidgetAbstract::WidgetAbstract(WidgetAbstract *parent) :
        TreeNamedNode(parent),
        m_manager(nullptr),
        m_position(0, 0),
        m_absolutePosition(0,0),
        m_isNeedToUpdateAbsolutePosition(false),
        m_size(0,0),
        m_isVisible(true),
        m_isEnabled(true),
        m_opacity(1.0f),
        m_padding{0,0,0,0}
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

void WidgetAbstract::setFocus() {
        if (s_focusedWidget == this) return;

        if (s_focusedWidget) s_focusedWidget->onWidgetClearFocus();             // тот кто был в фокусе, тот его потерял

        s_focusedWidget = this;                                                 // устанавливаем новый фокус
        onWidgetSetFocus();
}

void WidgetAbstract::clearFocus() {
        if (s_focusedWidget != this) return;

        s_focusedWidget = nullptr;
        onWidgetClearFocus();
}

void WidgetAbstract::setOpacity(float opacity) {
        m_opacity = glm::clamp(opacity, 0.0f, 1.0f);
        for (WidgetAbstract* node: children()) node->setOpacity(m_opacity);
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

void WidgetAbstract::setPadding(const WidgetPadding &padding) {
        m_padding = padding;
        notifyPositionUpdated();
}

void WidgetAbstract::setPadding(float left, float top, float right, float bottom) {
        setPadding({left, top, right, bottom});
}

void WidgetAbstract::setPadding(float offset) {
        setPadding({offset, offset, offset, offset});
}

WidgetPadding WidgetAbstract::padding() const {
        return m_padding;
}

void WidgetAbstract::alignHorizontal(WidgetHAlignment::Enum align) {
        IM_ASSERT(m_parent);

        switch(align) {
                case WidgetHAlignment::LEFT:
                        setLeft(0.0f);
                        break;
                case WidgetHAlignment::CENTER:
                        setLeft(parent()->contentWidth()/2 - width()/2);
                        break;
                case WidgetHAlignment::RIGHT:
                        setLeft(parent()->contentWidth() - width());
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
                        setTop(parent()->contentHeight()/2 - height()/2);
                        break;
                case WidgetVAlignment::BOTTOM:
                        setTop(parent()->contentHeight() - height());
                        break;
        }
}

Vec2 WidgetAbstract::absolutePosition() {
        updateAbsolutePosition();
        return m_absolutePosition;
}

void WidgetAbstract::setAbsolutePosition(const Vec2 &position) {
        if (m_parent) {
                setPosition(position - m_parent->absolutePosition());
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

Vec2 WidgetAbstract::contentSize() const {
        Vec2 result = size();
        result.x -= (m_padding.left + m_padding.right);         //отнимаем от ширины боковые стороны
        result.y -= (m_padding.top + m_padding.bottom);         //а от высоты - основания
        return result;
}

float WidgetAbstract::contentWidth() const {
        return width() - (m_padding.left + m_padding.right);
}

float WidgetAbstract::contentHeight() const {
        return height() - (m_padding.top + m_padding.bottom);
}

GuiManager* WidgetAbstract::manager() const {
        return m_manager;
}

WidgetAbstract* WidgetAbstract::focusedWidget() {
        return s_focusedWidget;
}

void WidgetAbstract::processRender() {
        if (!m_isVisible) return;

        onRender();
}

void WidgetAbstract::processUpdate() {
        onUpdate();
        for (WidgetAbstract* node: children()) node->processUpdate();
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
        for (WidgetAbstract* node: children()) {
                bool done = node->processMouseMove(oldX, oldY, newX, newY);
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
        for (WidgetAbstract* node: children()) {
                bool done = node->processMousePress(x, y, button);
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
        for (WidgetAbstract* node: children()) {
                bool done = node->processMouseRelease(x, y, button);
                if (done) return true;
        }

        // Обрабатываем сам виджет
        bool result = onMouseRelease(x, y, button);
        return result;
}

void WidgetAbstract::processGlobalMouseMove(int x, int y) {
        if (!m_isEnabled || !m_isVisible) return;

        onGlobalMouseMove(x, y);
        for (WidgetAbstract* node: children()) node->processGlobalMouseMove(x ,y);
}

void WidgetAbstract::processGlobalMousePress(int x, int y, char button) {
        if (!m_isEnabled || !m_isVisible) return;

        onGlobalMousePress(x, y, button);
        for (WidgetAbstract* node: children()) node->processGlobalMousePress(x ,y, button);
}

void WidgetAbstract::processGlobalMouseRelease(int x, int y, char button) {
        if (!m_isEnabled || !m_isVisible) return;

        onGlobalMouseRelease(x, y, button);
        for (WidgetAbstract* node: children()) {
                node->processGlobalMouseRelease(x ,y, button);
        }
}

bool WidgetAbstract::isInsideWidget(int x, int y) {
        Vec2 min = absolutePosition();
        Vec2 max = min + size();
        return (x >= min.x && y >= min.y) && (x <= max.x && y <= max.y);
}

void WidgetAbstract::renderChildren() {
        for (WidgetAbstract* node: children()) node->processRender();
}

void WidgetAbstract::onAttachChild(TreeNode *node) {
        notifyPositionUpdated();
}

void WidgetAbstract::onDetachChild(TreeNode *node) {
        notifyPositionUpdated();
}

void WidgetAbstract::notifyPositionUpdated() {
        m_isNeedToUpdateAbsolutePosition = true;
        for (WidgetAbstract* node: children()) {
                node->notifyPositionUpdated();
        }
}

void WidgetAbstract::updateAbsolutePosition() {
        WidgetAbstract* pnt = m_parent;

        if (m_isNeedToUpdateAbsolutePosition) {
                if (pnt) {
                        m_absolutePosition = pnt->absolutePosition() + Vec2(pnt->m_padding.left, pnt->m_padding.top) + m_position;
                } else {
                        m_absolutePosition = m_position;
                }

                m_isNeedToUpdateAbsolutePosition = false;
        }
}


} //namespace imEngine
