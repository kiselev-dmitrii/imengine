#include "Image.h"
#include <imEngine/Utils/Debug.h>

namespace imEngine {
namespace GAPI {

Image::Image() {
        ilGenImages(1, &m_handle);
}

Image::Image(const String &filename) {
        ilGenImages(1, &m_handle);
        load(filename);
}

Image::~Image() {
        ilDeleteImages(1, &m_handle);
}

void Image::load(const String &filename) {
        ilBindImage(m_handle);

        if (!ilLoadImage(filename.c_str())) {
                IM_ERROR("Cannot load image from file " << filename);
                return;
        }
        IM_LOG("Image was loaded from file: " << filename);
}

void Image::load(int width, int height, int depth, int numChannels, ILenum srcFormat, ILenum srcType, void *data) {
        ilBindImage(m_handle);

        ilTexImage(width, height, depth, numChannels, srcFormat, srcType, data);
}

void Image::save(const String &filename, bool overwrite) {
        ilBindImage(m_handle);

        // Устанавливаем флаг перезаписываемости
        if (overwrite) ilEnable(IL_FILE_OVERWRITE);
        else ilDisable(IL_FILE_OVERWRITE);

        // Сохраняем
        if (!ilSaveImage(filename.c_str())) {
                IM_ERROR("Can't save image to file " << filename);
                return;
        }
        IM_LOG("Image was saved to file: " << filename);
}

int Image::width() {
        ilBindImage(m_handle);
        return ilGetInteger(IL_IMAGE_WIDTH);
}

int Image::height() {
        ilBindImage(m_handle);
        return ilGetInteger(IL_IMAGE_HEIGHT);
}

int Image::depth() {
        ilBindImage(m_handle);
        return ilGetInteger(IL_IMAGE_DEPTH);
}

TextureSrcFormat::Enum Image::format() {
        ilBindImage(m_handle);
        return (TextureSrcFormat::Enum) ilGetInteger(IL_IMAGE_FORMAT);
}

TextureSrcType::Enum Image::type() {
        ilBindImage(m_handle);
        return (TextureSrcType::Enum) ilGetInteger(IL_IMAGE_TYPE);
}

void* Image::data() {
        ilBindImage(m_handle);
        return ilGetData();
}

} //namespace GAPI
} //namespace imEngine
