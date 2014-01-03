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
        void    setText(const String& text);
        /// Устанавливает новый шрифт
        void    setFont(const FontPtr& font);
        /// Устанавливает новую позицию
        void    setPosition(const Vec2& position);
        /// Устанавливает новую глубину
        /// Текст будет перекрываться другим, при включенном тесте буфера глубины
        void    setDepth(float depth);
        /// Устанавливает цвет текста
        void    setColor(const Vec3& color);
        /// Устанавливает окно, в котором отображается текст
        /// Нужно для получения текущего разрешения
        void    setWindow(Window* window);

        /// Рендерит текст с заданными настройками
        void    render();

private:
        /// Создает буфер и VAO и настраивает его
        void            initBuffer();
        /// Обновляет буфера вершин
        void            updateBuffer();
        /// Возвращает собранную программу.
        /// Используется для существования одного эземпляра Program для всех экземпляров Text
        static Program& program();

private:
        bool            m_needToUpdateBuffer;

        String          m_text;
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
