#ifndef TEXTURE1D_H
#define TEXTURE1D_H

#include "Texture.h"
#include "../../Utils/Types.h"

namespace imCore {

/** @brief Класс одномерной текстуры.
 *  Наследует основной класс Texture
 *  Имеет методы для загрузки и сохранение в файл
 */
class Texture1D : public Texture {
public:
        /// Создает пустую одномерную текстуру размера width (видеопамять выделяется)
        Texture1D(int width, TextureInternalFormat::Enum internal, TextureSrcType::Enum srcType, TextureSrcFormat::Enum srcFormat);
        /// Создает одномерную текстуру заполненую данными из src
        Texture1D(int width, TextureInternalFormat::Enum internal, TextureSrcType::Enum srcType, TextureSrcFormat::Enum srcFormat, GLvoid* src);
        /// Создает одномерную текстуру из файла изображения
        Texture1D(const String& filename);

        /// Загружает данные в текстуру из src
        void            upload(int width, TextureInternalFormat::Enum internal, TextureSrcType::Enum srcType, TextureSrcFormat::Enum srcFormat, GLvoid* src);
        /// Загружает данные в текстуру из изображения filename
        void            upload(const String& filename);
        /// Выделяет область памяти размера width
        void            allocate(int width, TextureInternalFormat::Enum internal, TextureSrcType::Enum srcType, TextureSrcFormat::Enum srcFormat);

        /// Сохраняет данные в виде изображения 1xWidth()
        void            save(const String& filename, bool overwrite = true);
};

} //namespace imCore

#endif // TEXTURE1D_H
