#include "Texture.h"
#include <imEngine/Utils/Debug.h>
#include <imEngine/Utils/GLUtils.h>

namespace imEngine {


GLuint Texture::s_boundHandle = 0;
int    Texture::s_currentUnit = 0;


Texture::Texture(TextureTarget::Enum target)  {
        m_handle = 0;
        m_width = m_height = m_depth = 0;
        m_numberOfImages = 0;
        m_wasMemoryAllocated = false;
        m_target = target;

        IM_GLCALL(glGenTextures(1, &m_handle));
        IM_LOG("Texture" << m_handle << ": created " << GLUtils::convertEnumToString(m_target));
        bind();                                         //первый bind определяет тип текстуры

        // По дефолту установим линейную фильтрацию
        setMinimizationFilter(TextureMinFilter::LINEAR);
        setMagnificationFilter(TextureMagFilter::LINEAR);
}

Texture::~Texture() {
        IM_GLCALL(glDeleteTextures(1, &m_handle));
        IM_LOG("Texture" << m_handle << ": destroyed");
}

GLuint Texture::handle() {
        return m_handle;
}

TextureTarget::Enum Texture::target() {
        return m_target;
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

uint Texture::numberOfImages() {
        return m_numberOfImages;
}

int Texture::numberOfChannels() {
        IM_ASSERT(m_wasMemoryAllocated);
        return SourceFormat::numComponents(m_srcFormat);
}

int Texture::sizeOfComponent() {
        IM_ASSERT(m_wasMemoryAllocated);
        return SourceType::sizeOf(m_srcType) ;
}

int Texture::sizeOfData() {
        return numberOfImages() * width() * height() * depth() * sizeOfComponent() * numberOfChannels();
}

bool Texture::wasMemoryAllocated() {
        return m_wasMemoryAllocated;
}

InternalFormat::Enum Texture::internalFormat() {
        IM_ASSERT(m_wasMemoryAllocated);
        return m_internalFormat;
}

SourceFormat::Enum Texture::sourceFormat() {
        IM_ASSERT(m_wasMemoryAllocated);
        return m_srcFormat;
}

SourceType::Enum Texture::sourceType() {
        IM_ASSERT(m_wasMemoryAllocated);
        return m_srcType;
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
        IM_ASSERT(m_wasMemoryAllocated);

        bind();
        IM_GLCALL(glGenerateMipmap(m_target));
}

void Texture::bind(int unit) {
        if (unit != s_currentUnit && unit != -1) {
                IM_GLCALL(glActiveTexture(GL_TEXTURE0 + unit));
                IM_GLCALL(glBindTexture(m_target, m_handle));
                s_currentUnit = unit;
                s_boundHandle = m_handle;
        }
        if (m_handle != s_boundHandle) {
                IM_GLCALL(glBindTexture(m_target, m_handle));
                s_boundHandle = m_handle;
        }
}

void Texture::unbind() {
        IM_GLCALL(glBindTexture(m_target, 0));
        s_boundHandle = 0;
}

std::shared_ptr<ubyte> Texture::data() {
        IM_ASSERT(m_wasMemoryAllocated);

        ubyte* data = new ubyte [sizeOfData()];
        IM_GLCALL(glGetTexImage(m_target, 0, m_srcFormat, m_srcType, data));
        return std::shared_ptr<ubyte>(data);
}

void Texture::updateTextureInformation(GLsizei width, GLsizei height, GLsizei depth, uint numberOfImages, InternalFormat::Enum internalFormat,
                                       SourceType::Enum srcType, SourceFormat::Enum srcFormat, bool wasMemoryAllocated) {
        m_width = width;
        m_height = height;
        m_depth = depth;
        m_numberOfImages = numberOfImages;
        m_internalFormat = internalFormat;
        m_srcType = srcType;
        m_srcFormat = srcFormat;
        m_wasMemoryAllocated = wasMemoryAllocated;
}


} //namespace imEngine
