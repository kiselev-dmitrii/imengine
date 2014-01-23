#ifndef TEXT_H
#define TEXT_H

#include "Widget.h"
#include <imEngine/FileContainers/Font.h>

namespace imEngine {

/** @brief Виджет-текст.
 *
 * Отображает текст в заданной позиции. При изменении текста, изменяется размер
 */
class Text : public WidgetAbstract {
public:
        /// Конструктор
        Text(const String& text, WidgetAbstract* parent);

        /// Устанавливает/возвращает текст
        void            setText(const String& text);
        const String&   text() const;

        /// Устанавливает/возврашает шрифт
        void            setFont(const FontPtr& font);
        FontPtr         font() const;

        /// Устанавливает/возвращате цвет шрифта
        void            setColor(const Vec3& color);
        Vec3            color() const;

        /// Устанавливает//возвращает вертикальный интервал
        void            setLineSpacingFactor(float factor);
        float           lineSpacingFactor() const;

        /// Возвращает расстояние между строками (от середин двух строк)
        uint            lineSpacing();

        /// Рендерит текст
        void            onRender();

private:
        /// Инициализирует программу
        void    initProgram();
        /// Инициализирует VBO и VAO.
        void    initBuffer();
        /// Обновление VBO
        void    updateBuffer();
        /// Расчитывает количество символов, которые будут визуализированы
        uint    calculateNumberOfVisibleChars(const String& str) const;
        /// Расчитывает размер текста
        Vec2    calculateSizeOfText(const String& str, FontPtr font, float lineSpacing) const;

private:
        String                  m_text;
        FontPtr                 m_font;
        Vec3                    m_color;
        float                   m_lineSpacingFactor;

        VertexArrayPtr          m_vao;
        VertexBufferPtr         m_vbo;
        bool                    m_isNeedToUpdateBuffer;

        uint                    m_numberOfVisibleChars;

        static ProgramPtr       s_program;
};


} //namespace imEngine

#endif // TEXT_H
