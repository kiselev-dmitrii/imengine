#include "Text.h"
#include "TextProgram.glsl"
#include <imEngine/Utils/Debug.h>

namespace imEngine {

struct SymbolGeometry {
        Vec2    offset;         // Относительная позиция каждого символа от начала слова в пикселях
        Vec2    size;           // Размер глифа на экране в пикселях
        Vec4    texCoords;      // Текстурные координаты глифа в атласе
};

Text::Text(const String &text, const FontPtr font, Window* window) :
        m_depth(0.0f),
        m_window(nullptr)
{
        setText(text);
        setFont(font);
        setWindow(window);

        setPosition(Vec2(0,0));
        setDepth(0.0f);
        setColor(Vec3(1,1,1));

        initBuffer();
        m_needToUpdateBuffer = true;
}

void Text::setText(const String &text) {
        if (text != m_text) {
                m_text = text;
                m_needToUpdateBuffer = true;
        }
}

void Text::setFont(const FontPtr &font) {
        if (font != m_font) {
                m_font = font;
                m_needToUpdateBuffer = true;
        }
}

void Text::setPosition(const Vec2 &position) {
        m_position = position;
}

void Text::setDepth(float depth) {
        m_depth = depth;
}

void Text::setColor(const Vec3 &color) {
        m_color = color;
}

void Text::setWindow(Window *window) {
        m_window = window;
}

void Text::render() {
        // Обновляем VBO если нужно
        if (m_needToUpdateBuffer) {
                updateBuffer();
                m_needToUpdateBuffer = false;
        }


        /// Устанавливаем текстуру и переменные и рендерим
        program().bind();
        m_font->texture()->bind(0);
        program().setUniform("u_texture", 0);
        program().setUniform("u_color", m_color);
        program().setUniform("u_windowSize", Vec2(m_window->size()));
        program().setUniform("u_position", m_position);
        program().setUniform("u_depth", m_depth);

        // Биндим наш буфер и рисуем
        m_vao->bind();
        Renderer::renderVertices(Primitive::POINT, m_text.size());
}

void Text::initBuffer() {
        m_vbo = VertexBufferPtr(new VertexBuffer());
        m_vao = VertexArrayPtr(new VertexArray());

        m_vao->bind();
                m_vbo->connect(program().attributeLocation("in_offset"), 2, GL_FLOAT,
                       offsetof(SymbolGeometry, offset), sizeof(SymbolGeometry));
                m_vbo->connect(program().attributeLocation("in_size"), 2, GL_FLOAT,
                       offsetof(SymbolGeometry, size) , sizeof(SymbolGeometry));
                m_vbo->connect(program().attributeLocation("in_texCoords"), 4, GL_FLOAT,
                       offsetof(SymbolGeometry, texCoords), sizeof(SymbolGeometry));
        m_vao->unbind();
}

Program& Text::program() {
        static bool wasInited = false;

        static Program program;
        if (!wasInited) {
                program.loadSource(textProgramSource);
                program.build();
                wasInited = true;
        }
        return program;
}

void fillSymbolGeometryArray(const String& text, FontPtr font, SymbolGeometry* result) {
        Vec2 offset(0,0);

        uint i = 0;
        for (char ch: text) {
                Glyph g = font->glyph(ch);

                result[i].size = g.size;
                result[i].texCoords = g.texCoords;
                result[i].offset = offset + Vec2(g.bearing.x, -g.bearing.y);    //bearing задан в правосторонней системе коорд, а мы работаем с оконной

                offset += Vec2(g.advance.x, -g.advance.y);                      //advance также в левосторонней задан
                ++i;
        }
}

void Text::updateBuffer() {
        SymbolGeometry data[m_text.size()];
        fillSymbolGeometryArray(m_text, m_font, data);
        m_vbo->load(data, sizeof(data), BufferUsage::STREAM_DRAW);
}


} // namespace imEngine
