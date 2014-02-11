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
        CubeTexture();
};


} //namespace imEngine

#endif // CUBETEXTURE_H
