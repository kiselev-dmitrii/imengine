#include "Texture1D.h"
#include "../../Utils/Debug.h"
#include "../../Utils/GLUtils.h"
#include "../../Utils/Types.h"
#include <IL/il.h>

namespace imCore {

Texture1D::Texture1D(TextureInternalFormat::Enum internal, int width, TextureSrcType::Enum srcType,
                     TextureSrcFormat::Enum srcFormat) : Texture(TextureTarget::TEXTURE_1D) {
        allocate(internal, width, srcType, srcFormat);
}

Texture1D::Texture1D(TextureInternalFormat::Enum internal, int width, TextureSrcType::Enum srcType,
                     TextureSrcFormat::Enum srcFormat, GLvoid *src) : Texture(TextureTarget::TEXTURE_1D) {
        upload(internal, width, srcType, srcFormat, src);
}

Texture1D::Texture1D(const String &filename) : Texture(TextureTarget::TEXTURE_1D){
        upload(filename);
}

void Texture1D::upload(TextureInternalFormat::Enum internal, int width, TextureSrcType::Enum srcType, TextureSrcFormat::Enum srcFormat, GLvoid *src) {
        bind();

        IM_GLCALL(glTexImage1D(m_target, 0, internal, width, 0, srcFormat, srcType, src));
        m_width = width;
        m_internalFormat = internal;
        m_srcType = srcType;
        m_srcFormat = srcFormat;
        m_wasMemoryAllocated = true;

        IM_LOG("Texture uploaded: handle: " << m_handle << ", target: " << GLUtils::convertEnumToString(m_target));
}

void Texture1D::upload(const String &filename) {
        // Загружаем изображение из path с помощью DevIL
        ILuint  imageID;
        ilGenImages(1, &imageID);
        ilBindImage(imageID);

        if (!ilLoadImage(filename.c_str())) {
                IM_ERROR("Cannot load image from file " << filename);
                return;
        }
        IM_LOG("Load image from file: " << filename);

        int format = ilGetInteger(IL_IMAGE_FORMAT);
        int width = ilGetInteger(IL_IMAGE_WIDTH);
        int type = ilGetInteger(IL_IMAGE_TYPE);
        ILubyte* src = ilGetData();
        upload((TextureInternalFormat::Enum)format, width, (TextureSrcType::Enum)type, (TextureSrcFormat::Enum)format, src);

        ilDeleteImages(1, &imageID);
}

void Texture1D::allocate(TextureInternalFormat::Enum internal, int width, TextureSrcType::Enum srcType, TextureSrcFormat::Enum srcFormat) {
        bind();

        IM_GLCALL(glTexImage1D(m_target, 0, internal, width, 0, srcFormat, srcType, NULL));
        m_width = width;
        m_internalFormat = internal;
        m_srcType = srcType;
        m_srcFormat = srcFormat;
        m_wasMemoryAllocated = true;

        IM_LOG("Texture allocated: handle: " << m_handle << ", target: " << GLUtils::convertEnumToString(m_target));
}

void Texture1D::save(const String &filename, bool overwrite) {
        IM_ASSERT(m_wasMemoryAllocated);
        IM_ASSERT(m_srcType == TextureSrcType::UBYTE);
        IM_ASSERT(m_srcFormat == TextureSrcFormat::RGB);

        // Создаем DevIL изображение
        ILuint imageID;
        ilGenImages(1, &imageID);
        ilBindImage(imageID);

        // Сохраняем в DevIL изображение из текстуры
        ubyte * data = new ubyte[3*m_width];
        this->bind();
        IM_GLCALL(glGetTexImage(m_target, 0, m_srcFormat, m_srcType, data));
        ilTexImage(m_width, 1, 1, 3, IL_RGB, IL_UNSIGNED_BYTE, data);
        delete [] data;

        // Сохраняем изображение
        if (overwrite) ilEnable(IL_FILE_OVERWRITE);
        else ilDisable(IL_FILE_OVERWRITE);

        if (!ilSaveImage(filename.c_str())) {
                ilDeleteImages(1, &imageID);
                IM_ERROR("Can't save image to file " << filename);
                return;
        }
        ilDeleteImages(1, &imageID);

        IM_LOG("Texture saved to file \'" << filename << "/': handle: " << m_handle << ", target: " << GLUtils::convertEnumToString(m_target));
}

} //namespace imCore
