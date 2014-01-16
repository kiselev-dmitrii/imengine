#include "Button.h"
#include <imEngine/Utils/Debug.h>

namespace imEngine {


Button::Button(const String &active, const String &hover, const String &pressed,
               const String &disabled, const String &focused, WidgetAbstract* parent) :
        BothStretchableTexturedWidget(active, parent),
        m_activeImage(active),
        m_hoverImage(hover),
        m_pressedImage(pressed),
        m_disabledImage(disabled),
        m_focusedImage(focused),
        m_isPressed(false)
{ }

bool Button::isPressed() {
        return m_isPressed;
}

bool Button::onMouseEnter(int x, int y) {
        setCurrentImage(m_hoverImage);
        return true;
}

bool Button::onMouseLeave(int x, int y) {
        if (m_isPressed) return false;
        setCurrentImage(m_activeImage);
        return true;
}

bool Button::onMousePress(int x, int y, char button) {
        if (button != MouseButton::LEFT) return false;

        m_isPressed = true;
        setCurrentImage(m_pressedImage);
        return true;
}

bool Button::onMouseRelease(int x, int y, char button) {
        if (button != MouseButton::LEFT) return false;

        if (m_isPressed) {
                onClick(this);
                m_isPressed = false;
        }

        setCurrentImage(m_activeImage);
        return true;
}

void Button::onGlobalMouseRelease(int x, int y, char button) {
        if (button != MouseButton::LEFT) return;

        m_isPressed = false;
        setCurrentImage(m_activeImage);
}

} //namespace imEngine
