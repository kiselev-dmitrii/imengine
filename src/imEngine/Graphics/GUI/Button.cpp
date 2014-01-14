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
        IM_TRACE("Processed");
        setCurrentImage(m_hoverImage);
        return true;
}

bool Button::onMouseLeave(int x, int y) {
        IM_TRACE("Processed");
        setCurrentImage(m_activeImage);
        return true;
}

} //namespace imEngine
