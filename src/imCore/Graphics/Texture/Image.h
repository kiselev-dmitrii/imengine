#ifndef IMAGE_H
#define IMAGE_H

#include "../../Utils/Types.h"
#include "Texture.h"
#include <IL/il.h>

namespace imCore {

/** @brief Класс представляет собой обертку для изображения из библиотеки DevIL.
 *
 *  Используется лишь для загрузки и сохранения текстур.
 *  Для управлением создания и удаления DevIL объекта используется принцип RAII.
 *  Предполагается что данный класс только для внутреннего использования.
 */
class Image {
public:
        /// Конструктор. Создает пустое изображение. Память не выделяется
        Image();
        /// Конструктор. Создает изображение из файла. Память выделяется
        Image(const String& filename);
        /// Деструктор
        ~Image();

        /// Загружает изображение из файла
        void                    load(const String& filename);
        /// Загружает изображение из data
        void                    load(int width, int height, int depth, int numChannels, ILenum srcFormat, ILenum srcType, void* data);
        /// Сохраняет изображение в файл
        void                    save(const String& filename, bool overwrite = true);

        /// Возвращает ширину изображеия
        int                     width();
        /// Возвращает высоту изображения
        int                     height();
        /// Возвращает глубину изображения
        int                     depth();

        /// Возвращает формат изображения
        TextureSrcFormat::Enum  format();
        /// Возвращает тип изображения
        TextureSrcType::Enum    type();
        /// Возвращает указатель на начало данных
        void*                   data();

private:
        ILuint          m_handle;
};

} //namespace imCore

#endif // IMAGE_H
