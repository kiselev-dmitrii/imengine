#include "Text.h"
#include "TextProgram.glsl"
#include <imEngine/Utils/Debug.h>

namespace imEngine {

struct SymbolGeometry {
        Vec3    position;       // Позиция верхнего левого угла глифа в коориданатах окна
        Vec2    size;           // Размер глифа на экране в пикселях
        Vec4    texCoords;      // Текстурные координаты глифа в атласе
};

Text::Text(const String &text, const FontPtr font) {
        setText(text);
        setFont(font);

        setPosition(Vec2(0,0));
        setDepth(0.0f);
        setColor(Vec3(1,1,1));

        initVBO();
        m_needToUpdateVBO = true;
}

void Text::setText(const String &text) {
        if (text == m_text) return;
        m_text = text;
        m_needToUpdateVBO = true;
}

void Text::setFont(const FontPtr &font) {
        if (font == m_font) return;
        m_font = font;

        // Привязываем новую текстуру к программе
        m_font->texture()->bind(0);
        program().setUniform("u_texture", 0);

        m_needToUpdateVBO = true;
}

void Text::setPosition(const Vec2 &position) {
        if (position == m_position) return;
        m_position = position;
        m_needToUpdateVBO = true;
}

void Text::setDepth(float depth) {
        if (depth == m_depth) return;
        m_depth = depth;
        m_needToUpdateVBO = true;
}

void Text::setColor(const Vec3 &color) {
        if (color == m_color) return;
        m_color = color;

        // Указываем новый цвет
        program().setUniform("u_color", m_color);
}

void Text::setWindow(Window *window) {
        m_window = window;
}

void Text::render() {
        // Обновляем VBO если нужно
        if (m_needToUpdateVBO) {
                updateVBO();
                m_needToUpdateVBO = false;
        }

        // Размер окна нужно постоянно обновлять, мало что там юзер с окном сделает
        program().setUniform("u_windowSize", Vec2(m_window->size()));

        // Биндим наш буфер и рисуем
        m_vbo->bind();
        Renderer::renderVertices(Primitive::POINT, m_text.size());
}

void Text::initVBO() {
        m_vbo = VertexBufferPtr(new VertexBuffer());
        m_vbo->connect(program().attributeLocation("in_position"), 3, GL_FLOAT,
                       offsetof(SymbolGeometry, position), sizeof(SymbolGeometry));
        m_vbo->connect(program().attributeLocation("in_size"), 2, GL_FLOAT,
                       offsetof(SymbolGeometry, size) , sizeof(SymbolGeometry));
        m_vbo->connect(program().attributeLocation("in_texCoords"), 4, GL_FLOAT,
                       offsetof(SymbolGeometry, texCoords), sizeof(SymbolGeometry));
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

void fillSymbolGeometryArray(const String& text, const Vec2& textPos, float textDepth, FontPtr font, SymbolGeometry* result) {
        Vec2 prevPos = textPos;

        uint i = 0;
        for (char ch: text) {
                Glyph g = font->glyph(ch);

                result[i].size = g.size;
                result[i].texCoords = g.texCoords;
                result[i].position = Vec3(prevPos + Vec2(g.bearing.x, -g.bearing.y), textDepth); //bearing задан в правосторонней системе коорд, а мы работаем с оконной

                prevPos += Vec2(g.advance.x, -g.advance.y);     //advance также в левосторонней задан
                ++i;
        }
}

void Text::updateVBO() {
        SymbolGeometry data[m_text.size()];
        fillSymbolGeometryArray(m_text, m_position, m_depth, m_font, data);
        m_vbo->load(data, sizeof(data), BufferUsage::STREAM_DRAW);
}


} // namespace imEngine
