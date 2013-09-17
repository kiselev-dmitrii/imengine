#include "Texture.h"
#include "../../Utils/Debug.h"

namespace imCore {

GLuint Texture::s_boundHandle = 0;

Texture::Texture(TextureTarget::Enum target) {
        m_target = target;
        IM_GLCALL(glGenTextures(1, &m_handle));
        bind();

        m_width = m_height = m_depth = 0;
}

Texture::~Texture() {
        IM_GLCALL(glDeleteTextures(1, &m_handle));
}

int Texture::width() {
        return m_width;
}

int Texture::height() {
        return m_height;
}

int Texture::depth() {
        return m_depth;
}

void Texture::setMagnificationFilter(TextureMagFilter::Enum filter) {
        bind();
        IM_GLCALL(glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, filter));
}

void Texture::setMinimizationFilter(TextureMinFilter::Enum filter) {
        bind();
        IM_GLCALL(glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, filter));
}

void Texture::setWrap(TextureWrapMode::Enum mode) {
        setWrapS(mode);
        setWrapT(mode);
        setWrapR(mode);
}

void Texture::setWrapS(TextureWrapMode::Enum mode) {
        bind();
        IM_GLCALL(glTexParameteri(m_target, GL_TEXTURE_WRAP_S, mode));
}

void Texture::setWrapT(TextureWrapMode::Enum mode) {
        bind();
        IM_GLCALL(glTexParameteri(m_target, GL_TEXTURE_WRAP_T, mode));
}

void Texture::setWrapR(TextureWrapMode::Enum mode) {
        bind();
        IM_GLCALL(glTexParameteri(m_target, GL_TEXTURE_WRAP_R, mode));
}

void Texture::setMinMipLevel(uint min) {
        bind();
        IM_GLCALL(glTexParameteri(m_target, GL_TEXTURE_BASE_LEVEL, min));
}

void Texture::setMaxMipLevel(uint max) {
        bind();
        IM_GLCALL(glTexParameteri(m_target, GL_TEXTURE_MAX_LEVEL, max));
}

void Texture::setCompareMode(TextureCompareMode::Enum mode) {
        bind();
        IM_GLCALL(glTexParameteri(m_target, GL_TEXTURE_COMPARE_MODE, mode));
}

void Texture::setCompareFunction(TextureCompareFunction::Enum func) {
        bind();
        IM_GLCALL(glTexParameteri(m_target, GL_TEXTURE_COMPARE_FUNC, func));
}

void Texture::setBorderColor(const Vec4 &color) {
        bind();
        IM_GLCALL(glTexParameterfv(m_target, GL_TEXTURE_BORDER_COLOR, &color[0]));
}

void Texture::generateMipmaps() {
        bind();
        IM_GLCALL(glGenerateMipmap(m_target));
}

void Texture::bind() {
        if (s_boundHandle != m_handle) {
                IM_GLCALL(glBindTexture(m_target, m_handle));
                s_boundHandle = m_handle;
        }
}

void Texture::unbind() {
        IM_GLCALL(glBindTexture(m_target, 0));
        s_boundHandle = 0;
}

GLuint Texture::rawTexture() {
        return m_target;
}

} //namespace imCore
