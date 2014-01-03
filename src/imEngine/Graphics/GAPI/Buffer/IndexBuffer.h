#ifndef INDEXBUFFER_H
#define INDEXBUFFER_H

#include <memory>
#include "BufferObject.h"

namespace imEngine {


/** @brief Класс предназначен для хранения индексов.
 *
 *  Индексы определяют порядок взятия очередной вершины из вершинного буфера,
 *  и используются при отрисовке сложных полигональных моделей.
 */
class IndexBuffer : public BufferObject {
public:
        /// Конструктор. Создает индексный буфер
        IndexBuffer();

        /// Привязывает буфер
        void            bind();
        /// Отвязывает буфер
        void            unbind();

private:
        static GLuint   s_boundHandle;

};


/** @brief Умный указатель с подсчетом ссылок для IndexBuffer
 */
typedef std::shared_ptr<IndexBuffer> IndexBufferPtr;


} //namespace imEngine

#endif // INDEXBUFFER_H
