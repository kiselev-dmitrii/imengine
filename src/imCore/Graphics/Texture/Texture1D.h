#ifndef TEXTURE1D_H
#define TEXTURE1D_H

#include "Texture.h"
#include "../../Utils/Types.h"

namespace imCore {

/** @brief Класс одномерной текстуры. НЕРЕАЛИЗОВАН.
 */
class Texture1D : public Texture {
public:
        /// Создает пустую одномерную текстуру размера width (видеопамять выделяется)
        Texture1D(TextureInternalFormat::Enum internal, int width);
        /// Создает одномерную текстуру заполненую данными из data
        Texture1D(TextureInternalFormat::Enum internal, int width, TextureSrcType::Enum srcType, TextureSrcFormat::Enum srcFormat, GLvoid* src);
        /// Создает одномерную текстуру из файла изображения
        Texture1D(const String& path);

        /// Загружает данные в текстуру из data
        void            upload(TextureInternalFormat::Enum internal, int width, TextureSrcType::Enum srcType, TextureSrcFormat::Enum srcFormat, GLvoid* src);
        /// Загружает данные в текстуру из текстуры path
        void            upload(const String& path);
        /// Выделяет область памяти размера width
        void            allocate(TextureInternalFormat::Enum internal, int width);

        /// Сохраняет данные в виде изображения 1xWidth()
        void            save(const String& path);
};

} //namespace imCore

#endif // TEXTURE1D_H
