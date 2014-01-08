#ifndef LABEL_H
#define LABEL_H

#include "Widget.h"
#include "Text.h"

namespace imEngine {


/** @brief Виджет представлющий собой прямоугольник с текстом
 */
class Label : public Widget {
public:
        /// Конструктор
        explicit        Label(const String& text, Widget* parent = 0);

        /// Устанавливает и возвращает текст
        void            setText(const String& text);
        const String&   text() const;

private:
        Text            m_text;
};


} //namespace imEngine

#endif // LABEL_H
