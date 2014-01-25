#ifndef LINEEDIT_H
#define LINEEDIT_H

#include "TexturedWidget.h"
#include "Text.h"

namespace imEngine {

/** @brief Курсор для LineEdit
 */
class LineEditCursor {
public:
        /// Конструктор
        LineEditCursor(Text* text, GraphicApplication* application);

        /// Управляет миганием курсора
        void    setBlinking(bool isBlinking);
        bool    isBlinking() const                                              { return m_isBlinking; }

        /// Устанавливает/возвращает видимость курсора в данный момент
        void    setVisible(bool isVisible);
        bool    isVisible() const                                               { return m_isVisible; }

        /// Скрывает курсор, если он показан и показывает, если скрыт
        void    blink();

        /// Обработка мигания курсора
        void    update();
        /// Рендерит курсор
        void    render();

private:
        bool                    m_isVisible;
        bool                    m_isBlinking;
        float                   m_prevBlinkTime;
        Text*                   m_text;
        GraphicApplication*     m_application;
};


/** @brief Виджет для ввода текста
 */
class LineEdit : public HStretchableTexturedWidget {
public:
        /// Конструктор
        /// Принимает на вход картинки для состояний виджета
        LineEdit(const String& active, const String& disabled, const String& focused,
                 WidgetAbstract* parent);

        /// Управляет видимостью курсора
        void    setCursorVisible(bool isVisible)                                { m_isCursorVisible = isVisible; }
        bool    isCursorEnabled() const                                         { return m_isCursorVisible; }

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

private:
        Text*           m_text;

        String          m_activeImage;
        String          m_disabledImage;
        String          m_focusedImage;

        LineEditCursor  m_cursor;
        bool            m_isCursorVisible;
};

} //namespace imEngine

#endif // LINEEDIT_H
