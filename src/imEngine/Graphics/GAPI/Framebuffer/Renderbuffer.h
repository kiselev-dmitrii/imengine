#ifndef RENDERBUFFER_H
#define RENDERBUFFER_H

#include <GL/glew.h>
#include <imEngine/Graphics/GAPI/Common.h>
#include <memory>

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
        void                    allocate(int width, int height, InternalFormat::Enum internal);
        /// Определяет, была ли выделена память
        bool                    wasMemoryAllocated();

        /// Возвращает размеры рендербуфера
        int                     width();
        int                     height();

        /// Привязывает/отвязывает рендербуфер к конетксту
        void                    bind();
        void                    unbind();

        /// Возвращает внутренний формат рендербуфера
        InternalFormat::Enum    internalFormat();
        /// Возвращает OGL дескриптор
        GLuint                  handle();

private:
        GLuint                  m_handle;
        InternalFormat::Enum    m_internalFormat;
        bool                    m_wasMemoryAllocated;

        int                     m_width;
        int                     m_height;

        static GLuint           s_boundHandle;

};


/** @brief Умный указатель на Renderbuffer
 */
typedef std::shared_ptr<Renderbuffer> RenderbufferPtr;


} //namespace imEngine

#endif // RENDERBUFFER_H
