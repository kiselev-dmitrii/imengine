#ifndef LABEL_H
#define LABEL_H

#include "Widget.h"
#include "Text.h"

namespace imEngine {

namespace LabelAlignment {
enum Enum {
        LEFT,
        RIGHT,
        CENTER
};
}


/** @brief Виджет представлющий собой прямоугольник с однострочным текстом
 *
 * Высота виджета будет равна высоте текста, ширину виджета можно указать
 * вручную. При этом, если ширина текста больше, чем ширина виджета, то
 * будет рендерится только нужная часть (это осуществляется путем использования
 * буфера трафарета).
 */
class Label : public WidgetAbstract, public HStretchableAbstract {
public:
        /// Конструктор
        explicit Label(const String& text, WidgetAbstract* parent);

        /// Рендерит виджет
        void            onRender();

        /// Устанавливает ширину прямоугольника (может быть любой)
        void            setWidth(float width)                                   { m_size.x = width; }

        /// Устанавливает и возвращает текст
        void            setText(const String& text)                             { m_text->setText(text); }
        const String&   text() const                                            { return m_text->text(); }

        /// Устанавливает и возвращает шрифт
        void            setFont(FontPtr font);
        FontPtr         font() const                                            { return m_text->font(); }

        /// Устанавливает и возвращает цвет
        void            setColor(const Vec3& color)                             { m_text->setColor(color); }
        Vec3            color() const                                           { return m_text->color(); }

        /// Устанавливает выравнивание
        void            setAlignment(LabelAlignment::Enum alignment)            { m_alignment = alignment; }

private:
        LabelAlignment::Enum    m_alignment;
        TextPtr                 m_text;
};


} //namespace imEngine

#endif // LABEL_H
