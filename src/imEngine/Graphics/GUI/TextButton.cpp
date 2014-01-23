#include "TextButton.h"

namespace imEngine {

TextButton::TextButton(const String &text, const String &active, const String &hover,
                       const String &pressed, const String &disabled, const String &focused, WidgetAbstract *parent) :
        Button(active, hover, pressed, disabled, focused, parent)
{
        m_text = new Text(text, this);
        m_text->alignHorizontal(WidgetHAlignment::CENTER);
        m_text->alignVertical(WidgetVAlignment::CENTER);
}

} //namespace imEngine
