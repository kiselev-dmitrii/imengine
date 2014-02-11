#include "Framebuffer.h"
#include <imEngine/Utils/Debug.h>
#include <imEngine/Utils/GLUtils.h>

namespace imEngine {

GLuint Framebuffer::s_boundHandle = 0;

Framebuffer::Framebuffer(FramebufferTarget::Enum target) {
        m_target = target;
        IM_GLCALL(glGenFramebuffers(1, &m_handle));
        IM_LOG("Framebuffer" << m_handle << ": created " << GLUtils::convertEnumToString(m_target));
}

Framebuffer::~Framebuffer() {
        IM_GLCALL(glDeleteFramebuffers(1, &m_handle));
        IM_LOG("Framebuffer" << m_handle << ": destroyed");
}

void Framebuffer::attachColorBuffer(uint i, Texture2D *texture, uint mipmapLevel) {
        bind();
        IM_GLCALL(glFramebufferTexture2D(m_target, GL_COLOR_ATTACHMENT0+i, GL_TEXTURE_2D, texture->handle(), mipmapLevel));
}

void Framebuffer::attachColorBuffer(uint i, CubeTexture *texture, CubeTextureSide::Enum side, uint mipmapLevel) {
        IM_TODO;
}

void Framebuffer::detachColorBuffer(uint i) {
        bind();
        IM_GLCALL(glFramebufferTexture2D(m_target, GL_COLOR_ATTACHMENT0+i, GL_TEXTURE_2D, 0, 0));
}

void Framebuffer::attachDepthBuffer(Texture2D *texture, uint mipmapLevel) {
        bind();
        IM_GLCALL(glFramebufferTexture2D(m_target, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture->handle(), mipmapLevel));
}

void Framebuffer::detachDepthBuffer() {
        bind();
        IM_GLCALL(glFramebufferTexture2D(m_target, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, 0, 0));
}

bool Framebuffer::isValid() {
        bind();

        GLenum status = IM_GLCALL(glCheckFramebufferStatus(m_target));
        if (status != GL_FRAMEBUFFER_COMPLETE) {
                IM_ERROR("Framebuffer" << m_handle << " is incomplete: " << GLUtils::convertEnumToString(status));
                return false;
        } else {
                return true;
        }
}

void Framebuffer::bind() {
        if (m_handle != s_boundHandle) {
                IM_GLCALL(glBindFramebuffer(m_target, m_handle));
                s_boundHandle = m_handle;
        }
}

void Framebuffer::unbind() {
        IM_GLCALL(glBindFramebuffer(m_target, 0));
        s_boundHandle = 0;
}

GLuint Framebuffer::handle() {
        return m_handle;
}

FramebufferTarget::Enum Framebuffer::target() {
        return m_target;
}

}

