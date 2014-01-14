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
        m_focusedImage(focused)
{ }

bool Button::onMouseEnter(int x, int y) {
        setCurrentImage(m_hoverImage);
        return true;
}

bool Button::onMouseLeave(int x, int y) {
        setCurrentImage(m_activeImage);
        return true;
}

bool Button::onMousePress(int x, int y, char button) {
        m_wasPressed = true;

        setCurrentImage(m_pressedImage);
        return true;
}

bool Button::onMouseRelease(int x, int y, char button) {
        if (m_wasPressed) {
                if (onClick) onClick(this);
                m_wasPressed = false;
        }

        setCurrentImage(m_activeImage);
        return true;
}

} //namespace imEngine
