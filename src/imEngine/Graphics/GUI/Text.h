#ifndef TEXT_H
#define TEXT_H

#include <imEngine/Graphics/GAPI/GAPI.h>
#include <imEngine/FileContainers/Font.h>
#include <imEngine/System/Window.h>

namespace imEngine {

class Text {
public:
        /// Конструктор
        Text(const String& text, const FontPtr font, Window* window);

        /// Устанавливает новый текст
        void            setText(const String& text);
        /// Возвращает текущий текст
        const String&   text() const                                                    { return m_text; }

        /// Устанавливает новый шрифт
        void            setFont(const FontPtr& font);
        /// Возвращает указатель на шрифт
        FontPtr         font() const                                                    { return m_font; }

        /// Устанавливает новую позицию в winSpace
        void            setPosition(const Vec2& position);
        /// Возвращает позицию текста
        Vec2            position() const                                                { return m_position; }

        /// Устанавливает новую глубину
        /// Текст будет перекрываться другим, при включенном тесте буфера глубины
        void            setDepth(float depth);
        /// Возвращает глубину текста
        float           depth() const                                                   { return m_depth; }

        /// Устанавливает цвет текста
        void            setColor(const Vec3& color);
        /// Возвращает текущий цвет текста
        Vec3            color() const                                                   { return m_color; }

        /// Устанавливает окно, в котором отображается текст
        /// Нужно для получения текущего разрешения
        void            setWindow(Window* window);
        /// Возвращает окно
        Window*         window() const                                                  { return m_window; }

        /// Рендерит текст с заданными настройками
        void            render();

private:
        /// Создает буфер и VAO и настраивает его
        void            initBuffer();
        /// Обновляет буфера вершин
        void            updateBuffer();
        /// Расчитывает количество символов, которые будут визуализированы
        uint            calcNumberOfVisibleChars(const String& str) const;

        /// Возвращает собранную программу.
        /// Используется для существования одного эземпляра Program для всех экземпляров Text
        static Program& program();

private:
        bool            m_needToUpdateBuffer;

        String          m_text;
        uint            m_numberOfVisibleChars;
        Vec2            m_position;
        float           m_depth;
        Vec3            m_color;
        Window*         m_window;

        FontPtr         m_font;
        VertexBufferPtr m_vbo;
        VertexArrayPtr  m_vao;

};

} //namespace imEngine

#endif // TEXT_H
