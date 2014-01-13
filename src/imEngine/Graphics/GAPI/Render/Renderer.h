#ifndef RENDERER_H
#define RENDERER_H

#include <GL/glew.h>

namespace imEngine {


namespace Primitive {
enum Enum {
        POINT                           = GL_POINTS,
        LINE                            = GL_LINES,
        TRIANGLE                        = GL_TRIANGLES,
        TRIANGLE_STRIP                  = GL_TRIANGLE_STRIP
};
}

namespace Buffer {
enum Enum {
        COLOR                           = GL_COLOR_BUFFER_BIT,
        DEPTH                           = GL_DEPTH_BUFFER_BIT,
        STENCIL                         = GL_STENCIL_BUFFER_BIT
};
}

/** @brief Статический класс, который представляет обертки для отрисовки объектов
 */
class Renderer {
public:
        /// Очищает текущий фреймбуфер и depth буфер
        static void     clearBuffers(GLbitfield flags = Buffer::COLOR | Buffer::DEPTH);

        /// Отрисовывает countVertices вершин из текущего VBO как примитывы type
        static void     renderVertices(Primitive::Enum type, GLsizei countVertices);
        /// Отрисовывает countIndices индексов из текущих VBO + IBO как примитивы типа type.
        /// Предполагается, что индексы имеют тип GL_UNSIGNED_INT
        static void     renderIndices(Primitive::Enum type, GLsizei countIndices);

        /// Включает буфер трафарета - дальнейший рендер осуществляется в него
        static void     beginStencilRendering();
        /// Рисует во фреймбуфер с использованием буфера трафарета
        static void     continueStencilRendering();
        /// Прекращает использование буфера трафарета
        static void     endStencilRendering();
};


} //namespace imEngine

#endif // RENDERER_H
