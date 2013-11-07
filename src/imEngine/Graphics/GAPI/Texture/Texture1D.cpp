#include "Texture1D.h"
#include "Image.h"
#include <imEngine/Utils/Debug.h>
#include <imEngine/Utils/GLUtils.h>

namespace imEngine {
namespace GAPI {

void Texture1D::create() {
        Texture::create(TextureTarget::TEXTURE_1D);
}

void Texture1D::load(int width, TextureInternalFormat::Enum internal, TextureSrcType::Enum srcType, TextureSrcFormat::Enum srcFormat, GLvoid *src) {
        bind();

        IM_GLCALL(glTexImage1D(m_target, 0, internal, width, 0, srcFormat, srcType, src));
        updateTextureInformation(width, 1, 1, 1, internal, srcType, srcFormat, true);

        IM_LOG("Texture" << m_handle << ": memory was allocated");
}

void Texture1D::load(const String &filename) {
        Image img(filename);
        load(img.width(), (TextureInternalFormat::Enum) img.format(), img.type(), img.format(), img.data());
}

void Texture1D::allocate(int width, TextureInternalFormat::Enum internal, TextureSrcType::Enum srcType, TextureSrcFormat::Enum srcFormat) {
        load(width, internal, srcType, srcFormat, NULL);
}

void Texture1D::save(const String &filename, bool overwrite) {
        IM_ASSERT(m_wasMemoryAllocated);

        Image img;
        img.load(width(), height(), depth(), numberOfChannels(), sourceFormat(), sourceType(), data().get());
        img.save(filename, overwrite);
}

} //namespace GAPI
} //namespace imEngine
