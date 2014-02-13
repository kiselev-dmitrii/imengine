#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <GL/glew.h>
#include <imEngine/Graphics/GAPI/Texture/Texture2D.h>
#include <imEngine/Graphics/GAPI/Texture/CubeTexture.h>
#include "Renderbuffer.h"

namespace imEngine {


/** @brief Точка привязки фрйембуфера
 */
namespace FramebufferTarget {
enum Enum {
        READ_ONLY = GL_READ_FRAMEBUFFER,
        WRITE_ONLY = GL_DRAW_FRAMEBUFFER,
        READ_WRITE = GL_FRAMEBUFFER
};
}


/** @brief Класс-обертка для FramebufferObject
 *
 * Используется для рендера в различные типы текстур
 */
class Framebuffer {
public:
        /// Конструктор
        explicit Framebuffer(FramebufferTarget::Enum target = FramebufferTarget::READ_WRITE);
        /// Деструктор
        ~Framebuffer();

        /// Приcоединяет/отсоединяет текстуру/рендер буфер в качестве i-того цветового буфера
        void                    attachColorBuffer(uint i, Texture2D* texture, uint mipmapLevel = 0);
        void                    attachColorBuffer(uint i, CubeTexture* texture, CubeTextureSide::Enum side, uint mipmapLevel = 0);
        void                    attachColorBuffer(uint i, Renderbuffer* renderbuffer);
        void                    detachColorBuffer(uint i);

        /// Привязывает текстуру/ренедрбуфер в качестве буфера глубины
        void                    attachDepthBuffer(Texture2D* texture, uint mipmapLevel = 0);
        void                    attachDepthBuffer(Renderbuffer* renderbuffer);
        void                    detachDepthBuffer();

        /// Проверяет буфер на валидность (подключены ли нужные буфера и т.д)
        bool                    isValid();

        /// Привязывает/отвязывает фреймбуфер
        void                    bind();
        void                    unbind();

        /// Возвращает OGL дескриптор
        GLuint                  handle();
        /// Возвращает точку привязки буфера
        FramebufferTarget::Enum target();

public:
        static uint             maxColorAttachments();

private:
        GLuint                  m_handle;
        FramebufferTarget::Enum m_target;

        static GLuint           s_boundHandle;
};


/** @brief Умный указатель на Renderbuffer
 */
typedef std::shared_ptr<Framebuffer> FramebufferPtr;


} //namespace imEngine

#endif // FRAMEBUFFER_H
