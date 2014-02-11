#include "Texture3D.h"
#include <imEngine/FileContainers/Image.h>
#include <imEngine/Utils/Debug.h>
#include <imEngine/Utils/GLUtils.h>
#include "string.h"
#include <fstream>

namespace imEngine {


Texture3D::Texture3D() : Texture(TextureTarget::TEXTURE_3D) {
}

void Texture3D::load(int width, int height, int depth, InternalFormat::Enum internal, SourceType::Enum srcType, SourceFormat::Enum srcFormat, GLvoid *src) {
        bind();

        IM_GLCALL(glTexImage3D(m_target, 0, internal, width, height, depth, 0, srcFormat, srcType, src));
        updateTextureInformation(width, height, depth, 1, internal, srcType, srcFormat, true);

        IM_LOG("Texture3D" << m_handle << ": memory was allocated");
}

void Texture3D::load(const StringList &filenames) {
        // Грузим изображения
        std::vector<ImagePtr> images;
        for (const String& filename: filenames) {
                ImagePtr img(new Image(filename));
                img->flipVertically();
                images.push_back(img);
        }

        int width = images[0]->width();
        int height = images[0]->height();
        int depth = images.size();
        InternalFormat::Enum internal = (InternalFormat::Enum) images[0]->format();
        SourceType::Enum srcType = images[0]->type();
        SourceFormat::Enum srcFormat = images[0]->format();

        // Выделяем память под данные
        int imgSize = images[0]->size();
        int totalSize = imgSize * images.size();
        ubyte* data = new ubyte[totalSize];
        for (uint i = 0; i < images.size(); ++i) {
                ImagePtr img = images[i];
                IM_ASSERT(img->width() == width);
                IM_ASSERT(img->height() == height);
                IM_ASSERT(img->type() == srcType);
                IM_ASSERT(img->format() == srcFormat);
                IM_ASSERT(img->size() == imgSize);

                ubyte* src = (ubyte*) images[i]->data();
                ubyte* dst = data + i * imgSize;
                memcpy(dst, src, imgSize);
        }

        load(width, height, depth, internal, srcType, srcFormat, data);
        delete data;
}

void Texture3D::load(int width, int height, int depth, InternalFormat::Enum internal, SourceType::Enum srcType, SourceFormat::Enum srcFormat, const String& filename) {
        std::ifstream infile(filename.c_str(), std::ios_base::binary);
        if (!infile) {
                IM_ERROR("Cannot load Texture3D: file " << filename << " not found");
                return;
        }

        /// Загружаем данные в data и закрываем файл
        int size =  width * height * depth * SourceFormat::numComponents(srcFormat) * SourceType::sizeOf(srcType);
        ubyte* data = new ubyte[size];
                infile.read(reinterpret_cast<char*>(data), size);
                load(width, height, depth, internal, srcType, srcFormat, data);
        delete data;
}

} //namespace imEngine
