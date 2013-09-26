#include "Texture2D.h"
#include "Image.h"
#include <imCore/Utils/Debug.h>
#include <imCore/Utils/GLUtils.h>

namespace imCore {

Texture2D::Texture2D(int width, int height, TextureInternalFormat::Enum internal, TextureSrcType::Enum srcType,
                     TextureSrcFormat::Enum srcFormat) : Texture(TextureTarget::TEXTURE_2D) {
        allocate(width, height, internal, srcType, srcFormat);
}

Texture2D::Texture2D(int width, int height, TextureInternalFormat::Enum internal, TextureSrcType::Enum srcType,
                     TextureSrcFormat::Enum srcFormat, GLvoid *src) : Texture(TextureTarget::TEXTURE_2D) {
        upload(width, height, internal, srcType, srcFormat, src);
}

Texture2D::Texture2D(const String &filename) : Texture(TextureTarget::TEXTURE_2D) {
        upload(filename);
}

void Texture2D::upload(int width, int height, TextureInternalFormat::Enum internal, TextureSrcType::Enum srcType, TextureSrcFormat::Enum srcFormat, GLvoid *src) {
        bind();

        IM_GLCALL(glTexImage2D(m_target, 0, internal, width, height, 0, srcFormat, srcType, src));
        m_width = width;
        m_height = height;
        m_depth = 1;
        m_internalFormat = internal;
        m_srcType = srcType;
        m_srcFormat = srcFormat;
        m_wasMemoryAllocated = true;

        IM_LOG("Texture memory was allocated: handle: " << m_handle << ", target: " << GLUtils::convertEnumToString(m_target));
}

void Texture2D::upload(const String &filename) {
        Image img(filename);
        upload(img.width(), img.height(), (TextureInternalFormat::Enum) img.format(), img.type(), img.format(), img.data());
}

void Texture2D::allocate(int width, int height, TextureInternalFormat::Enum internal, TextureSrcType::Enum srcType, TextureSrcFormat::Enum srcFormat) {
        upload(width, height, internal, srcType, srcFormat, NULL);
}

void Texture2D::save(const String &filename, bool overwrite) {
        IM_ASSERT(m_wasMemoryAllocated);

        Image img;
        img.load(width(), height(), depth(), numberOfChannels(), sourceFormat(), sourceType(), data().get());
        img.save(filename, overwrite);
}

} //namespace imCore
