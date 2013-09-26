#include "Texture2D.h"
#include "Image.h"
#include <imCore/Utils/Debug.h>
#include <imCore/Utils/GLUtils.h>

namespace imCore {

void Texture2D::create() {
        Texture::create(TextureTarget::TEXTURE_2D);
}

void Texture2D::load(int width, int height, TextureInternalFormat::Enum internal, TextureSrcType::Enum srcType, TextureSrcFormat::Enum srcFormat, GLvoid *src) {
        bind();

        IM_GLCALL(glTexImage2D(m_target, 0, internal, width, height, 0, srcFormat, srcType, src));
        updateTextureInformation(width, height, 1, internal, srcType, srcFormat, true);

        IM_LOG("Texture" << m_handle << ": memory was allocated");
}

void Texture2D::load(const String &filename) {
        Image img(filename);
        load(img.width(), img.height(), (TextureInternalFormat::Enum) img.format(), img.type(), img.format(), img.data());
}

void Texture2D::allocate(int width, int height, TextureInternalFormat::Enum internal, TextureSrcType::Enum srcType, TextureSrcFormat::Enum srcFormat) {
        load(width, height, internal, srcType, srcFormat, NULL);
}

void Texture2D::save(const String &filename, bool overwrite) {
        IM_ASSERT(m_wasMemoryAllocated);

        Image img;
        img.load(width(), height(), depth(), numberOfChannels(), sourceFormat(), sourceType(), data().get());
        img.save(filename, overwrite);
}

} //namespace imCore
