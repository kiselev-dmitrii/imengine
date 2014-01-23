#include "Font.h"
#include <imEngine/Utils/Debug.h>
#include <imEngine/Utils/StringUtils.h>

namespace imEngine {

FT_Library Font::s_ft;

Font::Font(const String &filename, uint size) {
        initFreeType();
        initFace(filename, size);
        initTexture();
        initFontInfo();
        m_maxHeight = calculateMaxHeight(m_glyphs);
        m_descenderHeight = calculateDescenderHeight(m_glyphs);
}

uint Font::maxHeight() const {
        return m_maxHeight;
}

uint Font::descenderHeight() const {
        return m_descenderHeight;
}

const Glyph& Font::glyph(char ch) const {
        IM_ASSERT(isVisibleGlyph(ch));

        return m_glyphs[ch-minChar];
}

bool Font::isVisibleGlyph(char ch) const {
        return (ch >= minChar && ch <= maxChar);
}

Texture2DPtr Font::texture() const {
        return m_texture;
}

uint Font::calculateWidthOfText(const String &text) const {
        uint width = 0;
        for (char ch: text) width += glyph(ch).advance.x;
        return width;
}

FontPtr Font::defaultFont() {
        return FontPtr(new Font("resources/font/FreeSans.ttf", 12));
}

void Font::initFreeType() {
        static bool wasInited = false;
        if (wasInited) return;

        if (FT_Init_FreeType(&s_ft)) {
                IM_ERROR("FreeType: Could not init library");
                return;
        }

        wasInited = true;
}

void Font::initFace(const String &filename, uint size) {
        if (FT_New_Face(s_ft, filename.c_str(), 0, &m_face)) {
                IM_ERROR("FreeType: Could not load font " << filename);
                return;
        }
        FT_Set_Pixel_Sizes(m_face, 0, size);
}

void Font::initTexture() {
        m_texture = Texture2DPtr(new Texture2D());

        // Создаем текстуру нужного размера
        IVec2 size = calculateTextureSize(m_face);
        m_texture->allocate(size.x, size.y, TextureInternalFormat::COLOR_NORM_1_COMP_8_BIT, TextureSrcType::UBYTE, TextureSrcFormat::R);
        m_texture->clear();

        // Заполняем текстуру глифами
        int currentX = 0;
        for (ubyte ch = minChar; ch <= maxChar; ++ch) {
                if(FT_Load_Char(m_face, ch, FT_LOAD_RENDER)) continue;

                FT_GlyphSlot g = m_face->glyph;

                // Изменяем порядок строк (FreeType отдает в перевернутом виде) и вставляем в структуру
                ubyte swapped[g->bitmap.width * g->bitmap.rows];
                swapRows(g->bitmap.buffer, g->bitmap.width, g->bitmap.rows, swapped);
                m_texture->insert(currentX, 0, g->bitmap.width, g->bitmap.rows, swapped, 1);

                currentX += g->bitmap.width;
        }

        // Убираем какую либо фильтрацию
        m_texture->setMagnificationFilter(TextureMagFilter::NEAREST);
        m_texture->setMinimizationFilter(TextureMinFilter::NEAREST);
}

void Font::initFontInfo() {
        m_glyphs.reserve(maxChar - minChar + 1);

        int currentX = 0;
        for (ubyte ch = minChar; ch <= maxChar; ++ch) {
                if(FT_Load_Char(m_face, ch, FT_LOAD_RENDER)) continue;
                FT_GlyphSlot g = m_face->glyph;

                Glyph glyph;

                glyph.size.x = g->bitmap.width;
                glyph.size.y = g->bitmap.rows;

                glyph.bearing.x = g->bitmap_left;
                glyph.bearing.y = g->bitmap_top;

                glyph.advance.x = g->advance.x >> 6;            // не знаю почему нужно делать сдвиг
                glyph.advance.y = g->advance.y >> 6;

                glyph.texCoords.x = float(currentX) / m_texture->width();
                glyph.texCoords.y = 0;

                glyph.texCoords.z = float(currentX + glyph.size.x) / m_texture->width();
                glyph.texCoords.w = float(glyph.size.y) / m_texture->height();

                m_glyphs.push_back(glyph);

                currentX += g->bitmap.width;
        }

}

uint Font::calculateMaxHeight(const GlyphList &glyphs) const {
        int maxHeight = 0;
        for (const Glyph& g: glyphs) maxHeight = std::max(maxHeight, g.size.y);
        return maxHeight;
}

uint Font::calculateDescenderHeight(const GlyphList &glyphs) const {
        int descenderHeight = 0;
        for (const Glyph& g: glyphs) {
                int curDescender = g.size.y - g.bearing.y;
                descenderHeight = std::max(descenderHeight, curDescender);
        }
        return descenderHeight;
}

void Font::swapRows(ubyte *input, uint width, uint height, ubyte* output) const {
        for (uint i=0; i<height; ++i) {
                mempcpy(output + (height-i-1)*width, input + i*width, width);
        }
}

IVec2 Font::calculateTextureSize(FT_Face face) const {
        IVec2 result(0,0);

        for (ubyte ch = minChar; ch <= maxChar; ++ch) {
                if(FT_Load_Char(face, ch, FT_LOAD_RENDER)) {
                        IM_ERROR("FreeType: Could not load character " << ch);
                        continue;
                }

                // Предполагается что глифы будут расположены в строчку
                result.x += face->glyph->bitmap.width;
                result.y = std::max(result.y, face->glyph->bitmap.rows);
        }

        return result;
}

/*
Font::Font(const String &filename, uint size) {
        if(initFreeType()) {
                loadFont(filename, size, &m_face);
                m_texture = generateTextureAtlas(m_face);
                m_glyphMetadata = generateGlyphMetadata(m_face, m_texture);
                m_maxHeight = calcMaxHeight(m_glyphMetadata);
                m_maxBaselineOffset = calcMaxBaselineOffset(m_glyphMetadata);
        }
}

Texture2DPtr Font::texture() const {
        return m_texture;
}

const Glyph& Font::glyph(char ch) const {
        IM_ASSERT(isGlyphVisible(ch));

        return m_glyphMetadata[ch-32];
}

uint Font::height() const {
        return m_maxHeight;
}

uint Font::maxBaselineOffset() const {
        return m_maxBaselineOffset;
}

IVec2 Font::calcSizeOfText(const String &text) const {
        StringList lines = StringUtils::split(text, "\n");
        uint textWidth = 0;
        uint textHeight = 0;
        for (auto &line: lines) {
                textWidth = std::max(calcWidthOfText(line), textWidth);
                textHeight += verticalInterval();
        }

        return IVec2(textWidth, textHeight);
}

float Font::calcWidthOfText(const String &text) const {
        uint width = 0;
        for (char ch: text) width += glyph(ch).advance.x;
        return width;
}

bool Font::isGlyphVisible(char ch) {
        return (ch >= 32 && ch < 128);
}

FontPtr Font::defaultFont() {
        return FontPtr(new Font("resources/font/FreeSans.ttf", 12));
}


uint Font::calcMaxHeight(const GlyphList &glyphs) {
        float height = 0;
        for (const Glyph& glyph: glyphs) height = std::max(height, glyph.size.y);
        return height;
}

uint Font::calcMaxBaselineOffset(const GlyphList &glyphs) {
        float offset = 0;
        for (const Glyph& glyph: glyphs) {
                float curOffset = glyph.size.y - glyph.bearing.y;
                offset = std::max(offset, curOffset);
        }
        return offset;
}
*/

} //namespace imEngine
