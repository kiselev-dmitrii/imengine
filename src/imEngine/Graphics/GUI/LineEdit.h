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

        /// Устанавливает/возвращает, включен ли курсор
        void    setCursorEnabled(bool isEnabled);
        bool    isCursorEnabled() const;

        /// Устанавливает/возвращает текст
        void    setText(const String& text);
        String  text() const;

        /// Обработка установки фокуса
        bool    onMousePress(int x, int y, char button);
        void    onGlobalMousePress(int x, int y, char button);
        /// Обработка действий при установке фокуса
        void    onWidgetSetFocus();
        void    onWidgetClearFocus();
        /// Обработка ввода
        void    onKeyPress(int key);
        /// Обработка мигания курсора
        void    onUpdate();
        /// Рендер текста и курсора
        void    onRender();

private:
        void    processCharKeys(char ch);
        void    processControlKeys(int key);
        void    blinkByCursor();
        void    setCursorVisible(bool isVisible);
        void    setCursorBlinking(bool isBlinking);

private:
        Text*   m_text;

        String  m_activeImage;
        String  m_disabledImage;
        String  m_focusedImage;

        /// Все касаемо курсора
        bool    m_isCursorVisible;
        bool    m_isCursorBlinking;
        float   m_prevBlinkTime;
        bool    m_isCursorEnabled;
};

} //namespace imEngine

#endif // LINEEDIT_H
