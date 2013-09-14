#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>

namespace imCore {

/** @brief Метод интерполяции, при уменьшении текстуры
 */
namespace TextureMinFilter {
enum Enum {
        NEAREST                         = GL_NEAREST,
        LINEAR                          = GL_LINEAR,
        NEAREST_MIPMAP_NEAREST          = GL_NEAREST_MIPMAP_NEAREST,
        LINEAR_MIPMAP_NEAREST           = GL_LINEAR_MIPMAP_NEAREST,
        NEAREST_MIPMAP_LINEAR           = GL_NEAREST_MIPMAP_LINEAR,
        LINEAR_MIPMAP_LINEAR            = GL_LINEAR_MIPMAP_LINEAR
};
}

/** @brief Метод интерполяции, при увеличении текстуры
 */
namespace TextureMagFilter {
enum Enum {
        NEAREST                         = GL_NEAREST,
        LINEAR                          = GL_LINEAR
};
}

/** @brief Режим оборачивания текстуры на полигон
 */
namespace TextureWrapMode {
enum Enum {
        REPEAT                  = GL_REPEAT,            ///< текстура повторяется
        REPEAT_MIRRORED         = GL_MIRRORED_REPEAT,   ///< текстура при повторении отражается
        CLAMP_TO_EDGE           = GL_CLAMP_TO_EDGE,     ///< граниченые пиксели текстуры растягиваются
        CLAMP_TO_BORDER         = GL_CLAMP_TO_BORDER    ///< граница заливается цветом border'а текстуры
};
}

/** @brief Базовый класс для работы с текстурами.
 *
 *  Большинство методов кэшируется. Действительно, OpenGL-объект создается лишь
 *  в последний момент, при вызове метода bind(). Если OGL-объект уже создан выполняется обычный
 *  glTextureBind. Кроме того, отслеживается id последней забинденного объекта.
 *  @todo Реализовать класс
 */
class Texture {
public:
        /// Конструктор
        explicit        Texture();
        /// Виртуальный деструктор
        virtual         ~Texture();

private:
};

} //namespace imCore

#endif // TEXTURE_H
