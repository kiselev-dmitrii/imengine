#include "TextButton.h"

namespace imEngine {

TextButton::TextButton(const String &text, const String &active, const String &hover,
                       const String &pressed, const String &disabled, const String &focused, WidgetAbstract *parent) :
        Button(active, hover, pressed, disabled, focused, parent)
{
        m_label = new Label(text, this);
        m_label->alignHorizontal(WidgetHAlignment::CENTER);
        m_label->alignVertical(WidgetVAlignment::CENTER);
}

} //namespace imEngine
