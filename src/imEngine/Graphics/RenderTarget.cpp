#include "RenderTarget.h"
#include <imEngine/Utils/Debug.h>

namespace imEngine {


const uint RenderTarget::s_maxColorBuffers = 16;

RenderTarget::RenderTarget(const IVec2 &size) :
        m_fbo(new Framebuffer()),
        m_size(size)
{
        m_colorTextureBuffers.resize(s_maxColorBuffers);
        m_colorRenderBuffers.resize(s_maxColorBuffers);
}

void RenderTarget::enableColorBuffer(uint i, InternalFormat::Enum internal, bool isTexture) {
        IM_ASSERT(i >= 0 && i < s_maxColorBuffers);

        if (isTexture) {
                Texture2DPtr texture(new Texture2D());
                SourceType::Enum srcType = InternalFormat::assumeSourceType(internal);
                SourceFormat::Enum srcFormat = InternalFormat::assumeSourceFormat(internal);
                texture->allocate(m_size.x, m_size.y, internal, srcType, srcFormat);

                disableColorBuffer(i);
                m_colorTextureBuffers[i] = texture;
                m_fbo->attachColorBuffer(i, texture.get());

        } else {
                RenderbufferPtr buff(new Renderbuffer());
                buff->allocate(m_size.x, m_size.y, internal);

                disableColorBuffer(i);
                m_colorRenderBuffers[i] = buff;
                m_fbo->attachColorBuffer(i, buff.get());
        }

        updateEnabledColorBufferList();
}

void RenderTarget::disableColorBuffer(uint i) {
        IM_ASSERT(i >= 0 && i < s_maxColorBuffers);

        if (m_colorTextureBuffers[i] || m_colorRenderBuffers[i]) m_fbo->detachColorBuffer(i);
        m_colorTextureBuffers[i].reset();
        m_colorRenderBuffers[i].reset();

        updateEnabledColorBufferList();
}

bool RenderTarget::isColorBufferEnabled(uint i) const {
        IM_ASSERT(i >= 0 && i < s_maxColorBuffers);

        return (m_colorTextureBuffers[i] || m_colorRenderBuffers[i]);
}

void RenderTarget::enableDepthBuffer(InternalFormat::Enum internal, bool isTexture) {
        IM_ASSERT(internal == InternalFormat::DEPTH_NORM_1_COMP_16_BIT  ||
                  internal == InternalFormat::DEPTH_NORM_1_COMP_24_BIT  ||
                  internal == InternalFormat::DEPTH_NORM_1_COMP_32_BIT  ||
                  internal == InternalFormat::DEPTH_FLOAT_1_COMP_32_BIT   );

        if (isTexture) {
                Texture2DPtr texture(new Texture2D());
                SourceType::Enum srcType = InternalFormat::assumeSourceType(internal);
                SourceFormat::Enum srcFormat = InternalFormat::assumeSourceFormat(internal);
                texture->allocate(m_size.x, m_size.y, internal, srcType, srcFormat);

                disableDepthBuffer();
                m_depthTextureBuffer = texture;
                m_fbo->attachDepthBuffer(texture.get());

        } else {
                RenderbufferPtr buff(new Renderbuffer()) ;
                buff->allocate(m_size.x, m_size.y, internal);

                disableDepthBuffer();
                m_depthRenderBuffer = buff;
                m_fbo->attachDepthBuffer(buff.get());
        }
}

void RenderTarget::disableDepthBuffer() {
        if (m_depthTextureBuffer || m_depthRenderBuffer) m_fbo->detachDepthBuffer();
        m_depthTextureBuffer.reset();
        m_depthRenderBuffer.reset();
}

bool RenderTarget::isDepthBufferEnabled() const {
        return (m_depthTextureBuffer || m_depthRenderBuffer);
}

Texture2DPtr RenderTarget::colorBufferTexture(uint i) const {
        return m_colorTextureBuffers[i];
}

Texture2DPtr RenderTarget::depthBufferTexture() const {
        return m_depthTextureBuffer;
}

void RenderTarget::setSize(const IVec2 &size) {
        if (size == m_size) return;
        m_size = size;

        /// Изменяем размеры всех вложений
        for (const Texture2DPtr& texture: m_colorTextureBuffers) {
                if (texture) texture->allocate(m_size.x, m_size.y, texture->internalFormat(), texture->sourceType(), texture->sourceFormat());
        }
        for (const RenderbufferPtr& buff: m_colorRenderBuffers) {
                if (buff) buff->allocate(m_size.x, m_size.y, buff->internalFormat());
        }
        if (m_depthTextureBuffer) {
                m_depthTextureBuffer->allocate(m_size.x, m_size.y, m_depthTextureBuffer->internalFormat(), m_depthTextureBuffer->sourceType(), m_depthTextureBuffer->sourceFormat());
        }
        if (m_depthRenderBuffer) {
                m_depthRenderBuffer->allocate(m_size.x, m_size.y, m_depthRenderBuffer->internalFormat());
        }
}

const IVec2& RenderTarget::size() const {
        return m_size;
}

void RenderTarget::bind() {
        m_fbo->bind();
        m_originalViewportSize = Renderer::viewportSize();
        IM_GLCALL(glDrawBuffers(m_enabledColorBuffers.size(), &(m_enabledColorBuffers[0])));
        Renderer::setViewportSize(m_size);
}

void RenderTarget::unbind() {
        Renderer::setViewportSize(m_originalViewportSize);
        m_fbo->unbind();
}

void RenderTarget::updateEnabledColorBufferList() {
        m_enabledColorBuffers.clear();
        for (uint i = 0; i < s_maxColorBuffers; ++i) {
                if (isColorBufferEnabled(i)) m_enabledColorBuffers.push_back(GL_COLOR_ATTACHMENT0 + i);
        }
}

} //namespace imEngine
