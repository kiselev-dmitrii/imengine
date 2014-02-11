#ifndef RENDERBUFFER_H
#define RENDERBUFFER_H

#include <GL/glew.h>
#include <imEngine/Graphics/GAPI/Common.h>

namespace imEngine {


/** @brief Класс-оберта для RenderbufferObject
 */
class Renderbuffer {
public:
        /// Создает редербуфер. Выделение памяти не происходит
        Renderbuffer();
        /// Уничтожает рендербуфер
        ~Renderbuffer();

        /// Выделяет память для рендербуфера
        void    allocate(int width, int height, InternalFormat::Enum internal);

        /// Привязывает/отвязывает рендербуфер к конетксту
        void    bind();
        void    unbind();

        /// Возвращает OGL дескриптор
        GLuint  handle();

private:
        GLuint          m_handle;
        static GLuint   s_boundHandle;

};


} //namespace imEngine

#endif // RENDERBUFFER_H
