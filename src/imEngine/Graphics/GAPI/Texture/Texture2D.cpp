#include "Texture2D.h"
#include <imEngine/FileContainers/Image.h>
#include <imEngine/Utils/Debug.h>
#include <imEngine/Utils/GLUtils.h>

namespace imEngine {

Texture2D::Texture2D() : Texture(TextureTarget::TEXTURE_2D) {
}

void Texture2D::load(int width, int height, InternalFormat::Enum internal, SourceType::Enum srcType, SourceFormat::Enum srcFormat, GLvoid *src, uint srcAlignment) {
        bind();

        if (srcAlignment !=4 ) IM_GLCALL(glPixelStorei(GL_UNPACK_ALIGNMENT, srcAlignment));             //если выравнивание не стандартное, то изменяем
        IM_GLCALL(glTexImage2D(m_target, 0, internal, width, height, 0, srcFormat, srcType, src));      //загружаем картинку
        if (srcAlignment !=4)  IM_GLCALL(glPixelStorei(GL_UNPACK_ALIGNMENT, 4));                        //восстанавливаем выравнивание

        updateTextureInformation(width, height, 1, 1, internal, srcType, srcFormat, true);

        IM_LOG("Texture2D" << m_handle << ": memory was allocated");
}

void Texture2D::load(const String &filename) {
        Image img(filename);
        img.flipVertically();                                                                           //OpenGL должен получать данные изображения в отраженном виде
        load(img.width(), img.height(), (InternalFormat::Enum) img.format(), img.type(), img.format(), img.data());
}

void Texture2D::allocate(int width, int height, InternalFormat::Enum internal, SourceType::Enum srcType, SourceFormat::Enum srcFormat) {
        load(width, height, internal, srcType, srcFormat, NULL);
}

void Texture2D::insert(int startX, int startY, int width, int height, GLvoid *src, uint srcAlignment, int level) {
        IM_ASSERT(startX + width <= m_width);
        IM_ASSERT(startY + height <= m_height);

        bind();

        if (srcAlignment !=4 ) IM_GLCALL(glPixelStorei(GL_UNPACK_ALIGNMENT, srcAlignment));
        IM_GLCALL(glTexSubImage2D(m_target, level, startX, startY, width, height, sourceFormat(), sourceType(), src));
        if (srcAlignment !=4)  IM_GLCALL(glPixelStorei(GL_UNPACK_ALIGNMENT, 4));

        IM_LOG("Texture2D" << m_handle << ": insert new subimage [" << startX << "-" <<  startX+width << ";" << startY << "-" << startY+height << "]");
}

void Texture2D::clear() {
        IM_ASSERT(m_wasMemoryAllocated);

        bind();
        std::vector<ubyte> zero(m_width * m_height * numberOfChannels(), 0);
        IM_GLCALL(glTexSubImage2D(m_target, 0, 0, 0, m_width, m_height, sourceFormat(), GL_UNSIGNED_BYTE, &zero[0]));

        IM_LOG("Texture2D" << m_handle << ": was cleared");
}

void Texture2D::save(const String &filename, bool overwrite) {
        IM_ASSERT(m_wasMemoryAllocated);

        Image img;
        img.load(width(), height(), depth(), numberOfChannels(), sourceFormat(), sourceType(), data().get());
        img.save(filename, overwrite);
}

} //namespace imEngine
