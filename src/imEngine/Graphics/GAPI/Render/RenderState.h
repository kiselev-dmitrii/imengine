#ifndef RENDERSTATE_H
#define RENDERSTATE_H

#include <GL/glew.h>

namespace imEngine {


/** @brief Режим смешивания
 */
namespace BlendMode {
enum Enum {
        NONE,
        ADD,
        SCREEN,
        LIGHTEN,
        MULTIPLY,
        DARKEN,
        BURN,
        ALPHA
};
}

/** @brief Функция сравнения глубины при DEPTH_TEST
 *
 * Записывает значение incomingDepth в буфер глубины с текущим значением depth, если
 * incomingDepth DepthFunct::Enum depth
 */
namespace DepthMode {
enum Enum {
        NONE                    = GL_NONE,      ///< отключает тест глубины

        NEVER                   = GL_NEVER,     ///< зачение incomingDepth никогда не записывается
        ALWAYS                  = GL_ALWAYS,    ///< значение incomingDepth всегда записывается

        LESS                    = GL_LESS,      ///< значение incomingDepth записывается, если incomingDepth < depth
        GREATER                 = GL_GREATER,   ///< значение incomingDepth записывается, если incomingDepth > depth

        EQUAL                   = GL_EQUAL,     ///< значение incomingDepth записывается, если incomingDepth = depth
        NOT_EQUAL               = GL_NOTEQUAL,  ///< значение incomingDepth записывается, если incomingDepth != depth

        LESS_OR_EQUAL           = GL_LEQUAL,    ///< значение incomingDepth записывается, если incomingDepth <= depth
        GREATER_OR_EQUAL        = GL_GEQUAL,    ///< значение incomingDepth записывается, если incomingDepth >= depth
};
}

/** @brief Режим отсечения плоскостей
 */
namespace CullMode {
enum Enum {
        NONE                    = GL_NONE,              ///< отключает CULL_FACE
        FRONT                   = GL_FRONT,             ///< отсекает передние грани
        BACK                    = GL_BACK               ///< отсекает задние грани
};
}

/** @brief Режим отрисовки примитивов
 */
namespace PolygonMode {
enum Enum {
        POINT                   = GL_POINT,
        LINE                    = GL_LINE,
        FILL                    = GL_FILL
};
}

/** @brief Состояние ренедера
 */
struct RenderState {
        BlendMode::Enum         blendMode;
        DepthMode::Enum         depthMode;
        CullMode::Enum          cullMode;
        PolygonMode::Enum       polygonMode;
};

} //namespace imEngine

#endif // RENDERSTATE_H
