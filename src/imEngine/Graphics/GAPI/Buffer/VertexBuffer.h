#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

#include <memory>
#include "BufferObject.h"

namespace imEngine {


/** @brief Класс используется для хранения массивов вершин в видеопамяти.
 *
 *  После создания вершинного буфера (VBO), данные привязываются к определенному атрибуту
 *  в вершинном шейдере. Во время атрисовки эти данные поступают на вход и обрабатываются
 *  GPU конвейером.
 */
class VertexBuffer : public BufferObject {
public:
        /// Конструктор. Создает вершинный буфер
        VertexBuffer();

        /// Привязвает буфер к вершинному атрибуту
        void            connect(GLuint attributeIndex, GLuint attributeSize, GLenum attributeType, GLsizei offset, GLsizei stride);

};


/** @brief Умный указатель с подсчетом ссылок для VertexBuffer
 */
typedef std::shared_ptr<VertexBuffer> VertexBufferPtr;


} //namespace imEngine

#endif // VERTEXBUFFER_H
