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
        Vec2    size;          //размер глифа в текселях
        Vec2    bearing;       //смещение от указываемой позиции до фактического символа в текселях
        Vec2    advance;       //смещение от одного символа до другого в текселях
        Vec4    texCoords;     //текстурные координаты
};
typedef std::vector<Glyph> GlyphList;


/** @brief Шрифт. Представляет собой обертку для библиотеки Freetype
 */
class Font {
public:
        /// Конструирует шрифт из файла со шрифтом filename размера size
        Font(const String& filename, uint size);

        /// Возвращает текстурный атлас со всеми символами
        Texture2DPtr            texture() const;
        /// Возвращает метаинформацию глифа
        const Glyph&            glyph(char ch) const;
        /// Возвращает расстояние между двумя строками
        uint                    verticalInterval() const;

        /// Расчитывает размер прямоугольника необходимый для рендеринга строки str
        IVec2                   calcSizeOfText(const String& text) const;
        /// Расчитывает ширину текста в пикселях
        uint                    calcWidthOfText(const String& text) const;

        /// Проверяет, является ли глиф видимым, то есть есть будет для него происходить рендер
        static bool             isGlyphVisible(char ch);

public:
        /// Статический метод. Возвращает указатель на стандартный шрифт
        static std::shared_ptr<Font> defaultFont();

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
        /// Меняет порядок строк в матрице
        static void             swapRows(ubyte* input, uint width, uint height, ubyte* output);

private:
        FT_Face                 m_face;
        Texture2DPtr            m_texture;
        GlyphList               m_glyphMetadata;

        static bool             s_isInited;
        static FT_Library       s_ft;

};


/** @brief Умный указатель для Font
 */
typedef std::shared_ptr<Font> FontPtr;

} //namespace imEngine

#endif // FONT_H
