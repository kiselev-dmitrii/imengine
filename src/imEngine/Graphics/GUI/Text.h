#ifndef TEXT_H
#define TEXT_H

#include <imEngine/Graphics/GAPI/GAPI.h>
#include <imEngine/FileContainers/Font.h>
#include <imEngine/System/Window.h>

namespace imEngine {

class Text {
public:
        Text(const String& text, const FontPtr font);

        void    setText(const String& text);
        void    setFont(const FontPtr& font);
        void    setPosition(const Vec2& position);
        void    setDepth(float depth);
        void    setColor(const Vec3& color);
        void    setWindow(Window* window);
        void    render();

private:
        /// Создает VBO, но не заполянет его
        void            initVBO();
        /// Обновляет VBO, если обновился текст, изменилась позиция или еще что
        void            updateVBO();

        /// Создает программу по требованию и не удаляет ее
        static Program& program();

private:
        bool            m_needToUpdateVBO;

        String          m_text;
        Vec2            m_position;
        float           m_depth;
        Vec3            m_color;
        Window*         m_window;

        FontPtr         m_font;
        VertexBufferPtr m_vbo;

};

} //namespace imEngine

#endif // TEXT_H
