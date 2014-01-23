#ifndef TEXTBUTTON_H
#define TEXTBUTTON_H

#include "Button.h"
#include "Text.h"

namespace imEngine {

/** @brief Кнопка с надписью
 */
class TextButton : public Button {
public:
        /// Конструктор.
        /// Принимает текст кнопки, и набор картинок, соответствующих состояниям кнопки
        TextButton(const String& text, const String& active, const String& hover, const String& pressed,
                   const String& disabled, const String& focused, WidgetAbstract* parent);

        /// Устанавливает/возвращает текст на кнопке
        void            setText(const String& text);
        const String&   text() const;

private:
        Text*   m_text;
        Vec2    m_padding;
};

} //namespace imEngine

#endif // TEXTBUTTON_H
