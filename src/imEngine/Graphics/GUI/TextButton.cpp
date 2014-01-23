#include "TextButton.h"

namespace imEngine {

TextButton::TextButton(const String &text, const String &active, const String &hover,
                       const String &pressed, const String &disabled, const String &focused, WidgetAbstract *parent) :
        Button(active, hover, pressed, disabled, focused, parent),
        m_padding(10, 5)
{
        m_text = new Text("", this);
        m_text->setColor(Vec3(0));

        setText(text);
}

void TextButton::setText(const String &text) {
        m_text->setText(text);

        if (width() < m_text->width() + m_padding.x) setWidth(m_text->width() + m_padding.x);
        if (height() < m_text->height() + m_padding.y) setHeight(m_text->height() + m_padding.y);
        m_text->alignHorizontal(WidgetHAlignment::CENTER);
        m_text->alignVertical(WidgetVAlignment::CENTER);
}

const String& TextButton::text() const {
        return m_text->text();
}

} //namespace imEngine
