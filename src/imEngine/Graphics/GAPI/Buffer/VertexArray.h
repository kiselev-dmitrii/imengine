#ifndef VERTEXARRAY_H
#define VERTEXARRAY_H

#include <GL/glew.h>

namespace imEngine {


/** @brief Класс-обертка для OGL объекта Vertex Array Object.
 *
 *  Может инкапсулировать в себе несколько VBO и IBO.
 *  Добавление в VAO осуществляется с помощью методов bind;
 *  @code
        vao.bind();
                vbo.bind();
                vbo.connect(ProgramAttributeLocations::POSITION, 3, GL_FLOAT, 3, 0);
                ibo.bind();
        vao.unbind();
 *  @endcode
 */
class VertexArray {
public:
        /// Конструктор. Создает VAO
        VertexArray();
        /// Деструктор. Уничтожает VAO
        ~VertexArray();

        /// Возращает OGL дескриптор
        GLuint          handle();

        /// Привязывает VAO
        void            bind();
        /// Отвязывает VAO
        void            unbind();

private:
        GLuint          m_handle;

        static GLuint   s_boundHandle;
};


} //namespace imEngine

#endif // VERTEXARRAY_H
