#include "Text.h"
#include "TextProgram.glsl"
#include <imEngine/Utils/Debug.h>
#include <imEngine/Utils/StringUtils.h>

namespace imEngine {

struct SymbolGeometry {
        Vec2    offset;         // Относительная позиция каждого символа от начала слова в пикселях
        Vec2    size;           // Размер глифа на экране в пикселях
        Vec4    texCoords;      // Текстурные координаты глифа в атласе
};

ProgramPtr Text::s_program;

Text::Text(const String &text, WidgetAbstract *parent) :
        WidgetAbstract(parent),
        m_text(text),
        m_font(Font::defaultFont()),
        m_color(0.5),
        m_lineSpacingFactor(1.5),
        m_isNeedToUpdateBuffer(true)
{
        m_size = calculateSizeOfText(m_text, m_font, lineSpacing());
        m_numberOfVisibleChars = calculateNumberOfVisibleChars(m_text);

        initProgram();
        initBuffer();
}

void Text::setText(const String &text) {
        if (text == m_text) return;

        m_text = text;

        m_numberOfVisibleChars = calculateNumberOfVisibleChars(m_text);
        m_size = calculateSizeOfText(m_text, m_font, lineSpacing());
        m_isNeedToUpdateBuffer = true;
}

const String& Text::text() const {
        return m_text;
}

void Text::setFont(const FontPtr &font) {
        if (font == m_font) return;

        m_font = font;

        m_size = calculateSizeOfText(m_text, m_font, lineSpacing());
        m_isNeedToUpdateBuffer = true;
}

FontPtr Text::font() const {
        return m_font;
}

void Text::setColor(const Vec3 &color) {
        m_color = color;
}

Vec3 Text::color() const {
        return m_color;
}

void Text::setLineSpacingFactor(float factor) {
        if (factor == m_lineSpacingFactor) return;

        m_lineSpacingFactor = factor;

        m_size = calculateSizeOfText(m_text, m_font, lineSpacing());
        m_isNeedToUpdateBuffer = true;
}

float Text::lineSpacingFactor() const {
        return m_lineSpacingFactor;
}

uint Text::lineSpacing() {
        return m_lineSpacingFactor * m_font->maxHeight();
}

void Text::onRender() {
        if (m_isNeedToUpdateBuffer) {
                updateBuffer();
                m_isNeedToUpdateBuffer = false;
        }

        /// Устанавливаем текстуру и переменные и рендерим
        s_program->bind();
        s_program->setUniform("u_windowSize", Vec2(manager()->window()->size()));
        s_program->setUniform("u_position", absolutePosition());
        m_font->texture()->bind(0);
        s_program->setUniform("u_texture", 0);
        s_program->setUniform("u_color", m_color);
        s_program->setUniform("u_opacity", m_opacity);

        // Биндим наш буфер и рисуем
        m_vao->bind();
        Renderer::renderVertices(Primitive::POINT, m_numberOfVisibleChars);
}

void Text::initProgram() {
        static bool wasInited = false;
        if (wasInited) return;

        s_program = ProgramPtr(new Program());
        s_program->loadSource(textProgramSource);
        s_program->build();

        wasInited = true;
}

void Text::initBuffer() {
        m_vbo = VertexBufferPtr(new VertexBuffer());
        m_vao = VertexArrayPtr(new VertexArray());

        m_vao->bind();
                m_vbo->connect(s_program->attributeLocation("in_offset"), 2, GL_FLOAT,
                       offsetof(SymbolGeometry, offset), sizeof(SymbolGeometry));
                m_vbo->connect(s_program->attributeLocation("in_size"), 2, GL_FLOAT,
                       offsetof(SymbolGeometry, size) , sizeof(SymbolGeometry));
                m_vbo->connect(s_program->attributeLocation("in_texCoords"), 4, GL_FLOAT,
                       offsetof(SymbolGeometry, texCoords), sizeof(SymbolGeometry));
        m_vao->unbind();
}

void Text::updateBuffer() {
        SymbolGeometry data[m_numberOfVisibleChars];

        // Заполняем массив data данными
        Vec2 symbolOrigin(0, m_font->maxHeight() - m_font->descenderHeight());                  //symbolOrigin - это точка на базовой линии, где лежит символ
        uint i = 0;
        for (char ch: m_text) {
                // Если символ видимый, то высчитываем его
                // координаты в буфер и переходим на следующий
                if (m_font->isVisibleGlyph(ch)) {
                        Glyph g = m_font->glyph(ch);

                        data[i].size = g.size;
                        data[i].texCoords = g.texCoords;
                        data[i].offset = symbolOrigin + Vec2(g.bearing.x, -g.bearing.y);   //bearing задан в правосторонней системе коорд, а мы работаем с оконной

                        symbolOrigin += Vec2(g.advance.x, -g.advance.y);                   //advance также в левосторонней задан

                        ++i;
                }

                // Если символ управляющий, то позиция следующего символа расчитывается по особому
                // сам же символ пропускается
                else {
                        if (ch == '\n') {
                                symbolOrigin.x = 0;
                                symbolOrigin.y += lineSpacing();
                        }
                }
        }

        // Загружаем данные в VBO
        m_vbo->load(data, sizeof(data), BufferUsage::STREAM_DRAW);
}

uint Text::calculateNumberOfVisibleChars(const String &str) const {
        // Дело в том, что возможны например символы переноса '\n',
        // и другие символы, которые не нужно рендерить и не нужно под них готовить буфер

        uint result = 0;
        for (char ch: str) if (m_font->isVisibleGlyph(ch)) ++result;
        return result;
}

Vec2 Text::calculateSizeOfText(const String &str, FontPtr font, float lineSpacing) const {
        StringList lines = StringUtils::split(str, "\n");
        uint textWidth = 0;
        uint textHeight = 0;

        for (auto &line: lines) {
                textWidth = std::max(font->calculateWidthOfText(line), textWidth);
                textHeight += lineSpacing;
        }
        textHeight = textHeight - lineSpacing + font->maxHeight();

        return Vec2(textWidth, textHeight);
}

} // namespace imEngine
