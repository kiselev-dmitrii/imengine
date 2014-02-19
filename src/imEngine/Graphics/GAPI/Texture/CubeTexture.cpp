#include "CubeTexture.h"
#include <imEngine/FileContainers/Image.h>
#include <imEngine/Utils/Debug.h>

namespace imEngine {


CubeTexture::CubeTexture() :
        Texture(TextureTarget::CUBEMAP)
{ }

void CubeTexture::load(const String &filename) {
        Image img(filename);
        img.flipVertically();

        /// Получаем данные
        int sideWidth = img.width();
        int sideHeight = img.height()/6;       //шесть изображений в высоту
        int sideSize = img.size()/6;
        InternalFormat::Enum internal = (InternalFormat::Enum) img.format();
        SourceType::Enum srcType = img.type();
        SourceFormat::Enum srcFormat = img.format();
        ubyte* src = (ubyte*) img.data();

        /// Проверяем и загружаем
        IM_ASSERT(sideWidth == sideHeight);
        for (uint i = 0; i < 6; ++i) {
                IM_GLCALL(glTexImage2D(CubeTextureSide::POSITIVE_X + i, 0, internal,
                                       sideWidth, sideHeight, 0, srcFormat, srcType, src + i*sideSize));
        }
        updateTextureInformation(sideWidth, sideHeight, 1, 6, internal, srcType, srcFormat, true);

        IM_LOG("TextureCube" << m_handle << ": memory was allocated");
}

void CubeTexture::load(const String &posx, const String &negx, const String &posy, const String &negy, const String &posz, const String &negz) {
        Image images[6] = {
                Image(posx), Image(negx),
                Image(posy), Image(negy),
                Image(posz), Image(negz)
        };

        /// Получаем данные
        int sideWidth = images[0].width();
        int sideHeight = images[0].height();
        InternalFormat::Enum internal = (InternalFormat::Enum) images[0].format();
        SourceType::Enum srcType = images[0].type();
        SourceFormat::Enum srcFormat = images[0].format();

        /// Проверяем и загружаем
        for (uint i = 0; i < 6; ++i) {
                IM_ASSERT(images[i].width() == images[i].height());
                IM_ASSERT(images[i].width() == sideWidth);
                IM_ASSERT(images[i].height() == sideHeight);
                IM_ASSERT(images[i].format() == srcFormat);
                IM_ASSERT(images[i].type() == srcType);

                IM_GLCALL(glTexImage2D(CubeTextureSide::POSITIVE_X + i, 0, internal,
                                       sideWidth, sideHeight, 0, srcFormat, srcType, images[i].data()));
        }
        updateTextureInformation(sideWidth, sideHeight, 1, 6, internal, srcType, srcFormat, true);

        IM_LOG("TextureCube" << m_handle << ": memory was allocated");
}

void CubeTexture::allocate(int width, int height, InternalFormat::Enum internal, SourceType::Enum srcType, SourceFormat::Enum srcFormat) {
        for (uint i = 0; i < 6; ++i) {
                IM_GLCALL(glTexImage2D(CubeTextureSide::POSITIVE_X + i, 0, internal,
                                       width, height, 0, srcFormat, srcType, NULL));
        }
        updateTextureInformation(width, height, 1, 6, internal, srcType, srcFormat, true);

        IM_LOG("TextureCube" << m_handle << ": memory was allocated");
}

} //namespace imEngine
