#include "Texture1D.h"
#include <imEngine/FileContainers/Image.h>
#include <imEngine/Utils/Debug.h>
#include <imEngine/Utils/GLUtils.h>

namespace imEngine {

Texture1D::Texture1D() : Texture(TextureTarget::TEXTURE_1D) {
}

void Texture1D::load(int width, InternalFormat::Enum internal, SourceType::Enum srcType,
                     SourceFormat::Enum srcFormat, GLvoid *src, uint srcAlignment) {
        bind();

        if (srcAlignment !=4 ) IM_GLCALL(glPixelStorei(GL_UNPACK_ALIGNMENT, srcAlignment));             //если выравнивание не стандартное, то изменяем
        IM_GLCALL(glTexImage1D(m_target, 0, internal, width, 0, srcFormat, srcType, src));
        if (srcAlignment !=4)  IM_GLCALL(glPixelStorei(GL_UNPACK_ALIGNMENT, 4));                        //восстанавливаем выравнивание

        updateTextureInformation(width, 1, 1, 1, internal, srcType, srcFormat, true);

        IM_LOG("Texture" << m_handle << ": memory was allocated");
}

void Texture1D::load(const String &filename) {
        Image img(filename);
        load(img.width(), (InternalFormat::Enum) img.format(), img.type(), img.format(), img.data());
}

void Texture1D::allocate(int width, InternalFormat::Enum internal, SourceType::Enum srcType, SourceFormat::Enum srcFormat) {
        load(width, internal, srcType, srcFormat, NULL);
}

void Texture1D::save(const String &filename, bool overwrite) {
        IM_ASSERT(m_wasMemoryAllocated);

        Image img;
        img.load(width(), height(), depth(), numberOfChannels(), sourceFormat(), sourceType(), data().get());
        img.save(filename, overwrite);
}

} //namespace imEngine
