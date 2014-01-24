#ifndef LINEEDIT_H
#define LINEEDIT_H

#include "TexturedWidget.h"
#include "Text.h"

namespace imEngine {

/** @brief Виджет для ввода текста
 */
class LineEdit : public HStretchableTexturedWidget {
public:
        /// Конструктор
        /// Принимает на вход картинки для состояний виджета
        LineEdit(const String& active, const String& disabled, const String& focused,
                 WidgetAbstract* parent);

        /// Обработка установки фокуса
        bool    onMousePress(int x, int y, char button);
        void    onGlobalMousePress(int x, int y, char button);

        /// Обработка действий при установке фокуса
        void    onWidgetSetFocus();
        void    onWidgetClearFocus();

        /// Обработка ввода
        void    onKeyPress(int key);

private:
        Text*   m_text;

        String  m_activeImage;
        String  m_disabledImage;
        String  m_focusedImage;
};

} //namespace imEngine

#endif // LINEEDIT_H
