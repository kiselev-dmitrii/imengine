#ifndef RENDERTARGET_H
#define RENDERTARGET_H

#include <imEngine/Math/Common.h>
#include <imEngine/Graphics/GAPI/GAPI.h>

namespace imEngine {


/** @brief Класс, предназначенный для удобного рендера в двумерную текстуру
 *  Поддерживает рендер в несколько текстур
 *
 *  При создании сразу содержит текстуры размера ренедтаргета.
 *  При изменении размера рендертаргета, автоматически пересоздаются и текстуры.
 *
 *  К сожалению, данный класс не поддерживает рендер в кубическую текстуру, и например
 *  чтобы реализовать динамические EnvriomentMap необходимо пользоваться FBO.
 */
class RenderTarget {
public:
        /// Конструктор.
        explicit RenderTarget(const IVec2& size);

        /// Включает/выключает цветовой буфер с индексом i.
        void            enableColorBuffer(uint i, InternalFormat::Enum internal, bool isTexture);
        void            disableColorBuffer(uint i);
        /// Определяет включен ли цветовой буфер с индесом i
        bool            isColorBufferEnabled(uint i) const;

        /// Включает/выключает буфер глубины
        void            enableDepthBuffer(InternalFormat::Enum internal, bool isTexture);
        void            disableDepthBuffer();
        /// Определяет включен ли буфер глубины
        bool            isDepthBufferEnabled() const;

        /// Возвращает цветовой буфер/буфер глубины, если это текстура
        Texture2DPtr    colorBufferTexture(uint i) const;
        Texture2DPtr    depthBufferTexture() const;

        /// Устанавливает/возвращает размер рендертаргета
        void            setSize(const IVec2& size);
        const IVec2&    size() const;

        /// Привязывает/отвязывает рендертаргет
        void            bind();
        void            unbind();

private:
        void            updateEnabledColorBufferList();

private:
        typedef std::vector<Texture2DPtr>       TextureList;
        typedef std::vector<RenderbufferPtr>    RenderbufferList;
        typedef std::vector<GLenum>             EnumList;

private:
        FramebufferPtr          m_fbo;
        IVec2                   m_size;

        TextureList             m_colorTextureBuffers;
        Texture2DPtr            m_depthTextureBuffer;

        RenderbufferList        m_colorRenderBuffers;
        RenderbufferPtr         m_depthRenderBuffer;

        EnumList                m_enabledColorBuffers;

        IVec2                   m_originalViewportSize;

        static const uint       s_maxColorBuffers;
};


} //namespace imEngine

#endif // RENDERTARGET_H
