#ifndef INDEXBUFFER_H
#define INDEXBUFFER_H

#include "BufferObject.h"

namespace imCore {

/** @brief Класс предназначен для хранения индексов.
 *
 *  Индексы определяют порядок взятия очередной вершины из вершинного буфера,
 *  и используются при отрисовке сложных полигональных моделей.
 */
class IndexBuffer : public BufferObject {
public:
        /// Создает индесный буфер
        void            create();

        /// Привязывает буфер
        void            bind();
        /// Отвязывает буфер
        void            unbind();

private:
        static GLuint   s_boundHandle;

};

} //namespace imCore

#endif // INDEXBUFFER_H
