#include "Texture2D.h"
#include <imCore/Utils/Debug.h>
#include <imCore/Utils/GLUtils.h>
#include <IL/il.h>

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
        m_internalFormat = internal;
        m_srcType = srcType;
        m_srcFormat = srcFormat;
        m_wasMemoryAllocated = true;

        IM_LOG("Texture memory was allocated: handle: " << m_handle << ", target: " << GLUtils::convertEnumToString(m_target));
}

void Texture2D::upload(const String &filename) {
        // Создаем DevIL изображение
        ILuint imageID;
        ilGenImages(1, &imageID);
        ilBindImage(imageID);

        // Загружаем изображение из файла
        if (!ilLoadImage(filename.c_str()))  {
                ilDeleteImages(1, &imageID);
                IM_ERROR("Cannot load image from file " << filename);
                return;
        }
        IM_LOG("Load image from file: " << filename);

        // Загружаем данные в видеопамять
        int width = ilGetInteger(IL_IMAGE_WIDTH);
        int height = ilGetInteger(IL_IMAGE_HEIGHT);
        int format = ilGetInteger(IL_IMAGE_FORMAT);
        int type = ilGetInteger(IL_IMAGE_TYPE);
        ILubyte* src = ilGetData();
        upload(width, height, (TextureInternalFormat::Enum)format, (TextureSrcType::Enum)type, (TextureSrcFormat::Enum)format, src);

        ilDeleteImages(1, &imageID);

}

void Texture2D::allocate(int width, int height, TextureInternalFormat::Enum internal, TextureSrcType::Enum srcType, TextureSrcFormat::Enum srcFormat) {
        upload(width, height, internal, srcType, srcFormat, NULL);
}

void Texture2D::save(const String &filename, bool overwrite) {
        IM_ASSERT(m_wasMemoryAllocated);
        IM_ASSERT(m_srcType == TextureSrcType::UBYTE ||
                  m_srcType == TextureSrcType::FLOAT);
        IM_ASSERT(m_srcFormat == TextureSrcFormat::DEPTH ||
                  m_srcFormat == TextureSrcFormat::R ||
                  m_srcFormat == TextureSrcFormat::RGBA ||
                  m_srcFormat == TextureSrcFormat::RGB);

        // Создаем DevIL изображение
        ILuint imageID;
        ilGenImages(1, &imageID);
        ilBindImage(imageID);

        // Определяем количество компонент в пикселе
        int numChannels = 3;
        if (m_srcFormat == TextureSrcFormat::DEPTH || m_srcFormat == TextureSrcFormat::R)  numChannels = 1;
        else if (m_srcFormat == TextureSrcFormat::RGBA) numChannels = 4;

        // Копируем данные из текстуры в DevIL
        this->bind();
        if (m_srcType == TextureSrcType::UBYTE) {
                ubyte* data = new ubyte[numChannels*m_width*m_height];
                IM_GLCALL(glGetTexImage(m_target, 0, m_srcFormat, m_srcType, data));
                ilTexImage(m_width, m_height, 1, numChannels, m_srcFormat, m_srcType, data);
                delete [] data;
        } else if (m_srcType == TextureSrcType::FLOAT) {
                float* data = new float[numChannels*m_width*m_height];
                IM_GLCALL(glGetTexImage(m_target, 0, m_srcFormat, m_srcType, data));
                ilTexImage(m_width, m_height, 1, numChannels, m_srcFormat, m_srcType, data);
                delete [] data;
        }

        // Сохраняем
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
