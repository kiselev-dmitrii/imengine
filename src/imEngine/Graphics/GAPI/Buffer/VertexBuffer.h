#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

#include "BufferObject.h"

namespace imEngine {
namespace GAPI {

/** @brief Класс используется для хранения массивов вершин в видеопамяти.
 *
 *  После создания вершинного буфера (VBO), данные привязываются к определенному атрибуту
 *  в вершинном шейдере. Во время атрисовки эти данные поступают на вход и обрабатываются
 *  GPU конвейером.
 */
class VertexBuffer : public BufferObject {
public:
        /// Создает вершинный буфер
        void            create();

        /// Привязывает буфер
        void            bind();
        /// Отвязывает буфер
        void            unbind();

        /// Привязвает буфер к вершинному атрибуту
        void            connect(GLuint attributeIndex, GLuint attributeSize, GLenum attributeType, GLsizei offset, GLsizei stride);

private:
        static GLuint   s_boundHandle;

};

} //namespace GAPI
} //namespace imEngine

#endif // VERTEXBUFFER_H
