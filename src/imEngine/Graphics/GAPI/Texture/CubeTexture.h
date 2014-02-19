#ifndef CUBETEXTURE_H
#define CUBETEXTURE_H

#include "Texture.h"

namespace imEngine {

/** @brief Стороны кубической текстуры
 */
namespace CubeTextureSide {
enum Enum {
        POSITIVE_X              = GL_TEXTURE_CUBE_MAP_POSITIVE_X,
        POSITIVE_Y              = GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
        POSITIVE_Z              = GL_TEXTURE_CUBE_MAP_POSITIVE_Z,

        NEGATIVE_X              = GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
        NEGATIVE_Y              = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
        NEGATIVE_Z              = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
};
}

/** @brief Класс кубической текстуры.
 *  @todo Реализовать
 */
class CubeTexture {
public:
        /// Конструктор. Создает кубическую текстуру
        CubeTexture();

        /// Загружает данные в текстуру из изображения.
        /// Предполагается что стороны расположены сверху вниз, в порядке POSX|NEGX|POSY|NEGY|POSZ|NEGZ
        void    load(const String& filename);
        /// Загружает данные в текстуру из изображений. Все изображения должны быть квадратными и одинаковыми по размеру
        void    load(const String& posx, const String& negx, const String& posy,
                     const String& negy, const String& posz, const String& negz);
        /// Выделяет область пустую область видеопамяти. Предыдущее содержание уничтожается
        void    allocate(int width, int height, InternalFormat::Enum internal,
                         SourceType::Enum srcType, SourceFormat::Enum srcFormat);
};


} //namespace imEngine

#endif // CUBETEXTURE_H
