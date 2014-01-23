#ifndef FONT_H
#define FONT_H

#include <imEngine/Graphics/GAPI/Texture/Texture2D.h>
#include <memory>
#include <ft2build.h>
#include FT_FREETYPE_H

namespace imEngine {

/** @brief Информация о глифе
 *
 *  Все вектора даны в правой системе координат (как в OGL)
 */
struct Glyph {
        IVec2   size;          //размер глифа в пикселях
        IVec2   bearing;       //смещение от указываемой позиции до фактического символа в пикселях
        IVec2   advance;       //смещение от одного символа до другого в пикселях
        Vec4    texCoords;     //текстурные координаты
};
typedef std::vector<Glyph> GlyphList;


/** @brief Умный указатель для Font
 */
class Font;
typedef std::shared_ptr<Font> FontPtr;


/** @brief Контейнер-загрузчик для шрифта.
 *
 * Основан на библиотеке FreeType
 */
class Font {
public:
        /// Конструирует шрифт из файла со шрифтом filename размера size
        Font(const String& filename, uint size);

        /// Возвращает максимальную высоту из всех глифов
        uint            maxHeight() const;
        /// Возвращает максимальное возвышение глифов над базовой линией
        uint            ascenderHeight() const;
        /// Возвращает максимальное провисание глифов под базовой линией
        uint            descenderHeight() const;
        /// Возвращает информацию о глифе ch
        const Glyph&    glyph(char ch) const;
        /// Определеяет, есть ли глиф ch в шрифте
        bool            isVisibleGlyph(char ch) const;

        /// Возвращает текстуру со всеми глифами шрифта
        Texture2DPtr    texture() const;

        /// Расчитывает ширину текста в пикселях
        uint            calculateWidthOfText(const String& text) const;

public:
        /// Возвращает шрифт по умолчанию
        static FontPtr  defaultFont();

private:
        /// Инициализирует FreeType, если надо
        void            initFreeType();
        /// Создает шрифт
        void            initFace(const String &filename, uint size);
        /// Создает текстуру по шрифту
        void            initTexture();
        /// Заполняет информацию о шрифте
        void            initFontInfo();

        /// Меняет порядок строк в матрице
        void            swapRows(ubyte* input, uint width, uint height, ubyte* output) const;
        /// Определяет размер текстуры по шрифту
        IVec2           calculateTextureSize(FT_Face face) const;

private:
        /// Минимальный и максимальный номер символа, для которого генерируется глифы
        static const char       minChar = 32;
        static const char       maxChar = 127;

        static FT_Library       s_ft;
        FT_Face                 m_face;
        Texture2DPtr            m_texture;
        GlyphList               m_glyphs;
};

/*
class Font {
public:
        /// Конструирует шрифт из файла со шрифтом filename размера size
        Font(const String& filename, uint size);

        /// Возвращает максимальную высоту глифа
        uint                    height() const;
        /// Возвращает максимальное провисание глифов под базовой линией (descender height)
        uint                    descender() const;
        uint                    accender() const;
        /// Возвращает метаинформацию глифа ch
        const Glyph&            glyph(char ch) const;
        /// Возвращает текстурный атлас со всеми символами
        Texture2DPtr            texture() const;


        /// Проверяет, является ли глиф видимым, то есть есть будет для него происходить рендер
        static bool             isGlyphVisible(char ch);

public:
        /// Статический метод. Возвращает указатель на стандартный шрифт
        static std::shared_ptr<Font> defaultFont();

private:


private:
        /// Инициализирует библиотеку
        static bool             initFreeType();
        /// Загружает шрифт
        bool                    loadFont(const String& filename, uint size, FT_Face* face);

        /// Генерирует текстуру
        static Texture2DPtr     generateTextureAtlas(FT_Face face);
        /// Расчитывает размер текстуры. Глифы расположены в строчку
        static IVec2            calcSizeOfTextureAtlas(FT_Face face);
        /// Получает информацию о глифах
        static GlyphList        generateGlyphMetadata(FT_Face face, Texture2DPtr texture);
        /// Считает максимальную высоту символов
        static uint             calcMaxHeight(const GlyphList& glyphs);
        /// Считает максимальное сдвиг по вертикали
        static uint             calcMaxBaselineOffset(const GlyphList& glyphs);

        /// Меняет порядок строк в матрице
        static void             swapRows(ubyte* input, uint width, uint height, ubyte* output);

private:
        FT_Face                 m_face;
        Texture2DPtr            m_texture;
        GlyphList               m_glyphMetadata;
        float                   m_maxHeight;
        float                   m_maxBaselineOffset;

        static bool             s_isInited;
        static FT_Library       s_ft;

};

 */

} //namespace imEngine

#endif // FONT_H
