#include "Font.h"
#include <imEngine/Utils/Debug.h>

namespace imEngine {

bool Font::s_isInited = false;
FT_Library Font::s_ft;

Font::Font(const String &filename, uint size) {
        if(initFreeType()) {
                loadFont(filename, size, &m_face);
                m_texture = generateTextureAtlas(m_face);
                m_glyphMetadata = generateGlyphMetadata(m_face, m_texture);
        }
}

Texture2DPtr Font::texture() const {
        return m_texture;
}

const Glyph& Font::glyph(char ch) const {
        IM_ASSERT(ch >= 32 && ch <= 128);

        return m_glyphMetadata[ch-32];
}

GlyphList Font::generateGlyphMetadata(FT_Face face, Texture2DPtr texture) {
        GlyphList result;
        result.reserve(128-32);

        int currentX = 0;
        for (ubyte ch = 32; ch < 128; ++ch) {
                if(FT_Load_Char(face, ch, FT_LOAD_RENDER)) continue;

                FT_GlyphSlot g = face->glyph;

                Glyph glyph;
                glyph.size.x = g->bitmap.width;
                glyph.size.y = g->bitmap.rows;

                glyph.bearing.x = g->bitmap_left;
                glyph.bearing.y = g->bitmap_top;

                glyph.advance.x = g->advance.x >> 6;            // не знаю почему нужно делать сдвиг
                glyph.advance.y = g->advance.y >> 6;

                glyph.texCoords.x = float(currentX) / texture->width();
                glyph.texCoords.y = 0;

                glyph.texCoords.z = float(currentX + glyph.size.x) / texture->width();
                glyph.texCoords.w = float(glyph.size.y) / texture->height();

                currentX += g->bitmap.width;

                result.push_back(glyph);
        }

        return result;
}

Texture2DPtr Font::generateTextureAtlas(FT_Face face) {
        Texture2DPtr texture(new Texture2D());

        // Создаем текстуру нужного размера
        IVec2 size = calcSizeOfTextureAtlas(face);
        texture->allocate(size.x, size.y, TextureInternalFormat::COLOR_NORM_1_COMP_8_BIT, TextureSrcType::UBYTE, TextureSrcFormat::R);
        texture->clear();

        // Заполняем текстуру глифами
        int currentX = 0;
        for (ubyte ch = 32; ch < 128; ++ch) {
                if(FT_Load_Char(face, ch, FT_LOAD_RENDER)) continue;

                FT_GlyphSlot g = face->glyph;

                // Изменяем порядок строк (FreeType отдает в перевернутом виде) и вставляем в структуру
                ubyte swapped[g->bitmap.width * g->bitmap.rows];
                swapRows(g->bitmap.buffer, g->bitmap.width, g->bitmap.rows, swapped);
                texture->insert(currentX, 0, g->bitmap.width, g->bitmap.rows, swapped, 1);

                currentX += g->bitmap.width;
        }

        // Устанавилваем линейную фильтрацию
        texture->setMagnificationFilter(TextureMagFilter::LINEAR);
        texture->setMinimizationFilter(TextureMinFilter::LINEAR);

        return texture;
}

void Font::swapRows(ubyte *input, uint width, uint height, ubyte* output) {
        for (uint i=0; i<height; ++i) {
                mempcpy(output + (height-i-1)*width, input + i*width, width);
        }
}

IVec2 Font::calcSizeOfTextureAtlas(FT_Face face) {
        IVec2 result(0,0);

        for (ubyte ch = 32; ch < 128; ++ch) {
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

bool Font::loadFont(const String &filename, uint size, FT_Face* face) {
        if (FT_New_Face(s_ft, filename.c_str(), 0, face)) {
                IM_ERROR("FreeType: Could not load font");
                return false;
        }
        FT_Set_Pixel_Sizes(*face, 0, size);

        return true;
}

bool Font::initFreeType() {
        if (s_isInited) return true;

        if (FT_Init_FreeType(&s_ft)) {
                IM_ERROR("FreeType: Could not init library");
                return false;
        } else {
                s_isInited = true;
                return true;
        }
}

} //namespace imEngine
