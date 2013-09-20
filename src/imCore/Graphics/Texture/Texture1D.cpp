#include "Texture1D.h"
#include "Image.h"
#include "../../Utils/Debug.h"
#include "../../Utils/GLUtils.h"
#include "../../Utils/Types.h"

namespace imCore {

Texture1D::Texture1D(int width, TextureInternalFormat::Enum internal, TextureSrcType::Enum srcType,
                     TextureSrcFormat::Enum srcFormat) : Texture(TextureTarget::TEXTURE_1D) {
        allocate(width, internal, srcType, srcFormat);
}

Texture1D::Texture1D(int width, TextureInternalFormat::Enum internal, TextureSrcType::Enum srcType,
                     TextureSrcFormat::Enum srcFormat, GLvoid *src) : Texture(TextureTarget::TEXTURE_1D) {
        upload(width, internal, srcType, srcFormat, src);
}

Texture1D::Texture1D(const String &filename) : Texture(TextureTarget::TEXTURE_1D){
        upload(filename);
}

void Texture1D::upload(int width, TextureInternalFormat::Enum internal, TextureSrcType::Enum srcType, TextureSrcFormat::Enum srcFormat, GLvoid *src) {
        bind();

        IM_GLCALL(glTexImage1D(m_target, 0, internal, width, 0, srcFormat, srcType, src));
        m_width = width;
        m_height = 1;
        m_depth = 1;
        m_internalFormat = internal;
        m_srcType = srcType;
        m_srcFormat = srcFormat;
        m_wasMemoryAllocated = true;

        IM_LOG("Texture memory was allocated: handle: " << m_handle << ", target: " << GLUtils::convertEnumToString(m_target));
}

void Texture1D::upload(const String &filename) {
        Image img(filename);
        upload(img.width(), (TextureInternalFormat::Enum) img.format(), img.type(), img.format(), img.data());
}

void Texture1D::allocate(int width, TextureInternalFormat::Enum internal, TextureSrcType::Enum srcType, TextureSrcFormat::Enum srcFormat) {
        upload(width, internal, srcType, srcFormat, NULL);
}

void Texture1D::save(const String &filename, bool overwrite) {
        IM_ASSERT(m_wasMemoryAllocated);

        Image img;
        img.load(width(), height(), depth(), numberOfChannels(), sourceFormat(), sourceType(), rawData().get());
        img.save(filename, overwrite);
}

} //namespace imCore
