#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include "Texture.h"
#include "../../Utils/Types.h"

namespace imCore {

/** @brief Класс двумерной текстуры.
 *  Позволяет загружать и сохранять текстуру в файл.
 */
class Texture2D : public Texture {
public:
        /// Создает двумерную текстуру
        void            create();

        /// Загружает данные в текстуру из src
        void            load(int width, int height, TextureInternalFormat::Enum internal, TextureSrcType::Enum srcType, TextureSrcFormat::Enum srcFormat, GLvoid* src);
        /// Загружает данные в текстуру из зображения filename
        void            load(const String& filename);
        /// Выделяет область пустую область видеопамяти. Предыдущее содержание уничтожается
        void            allocate(int width, int height, TextureInternalFormat::Enum internal, TextureSrcType::Enum srcType, TextureSrcFormat::Enum srcFormat);

        /// Сохраняет данные в изображение filename
        void            save(const String& filename, bool overwrite = true);
};

} //namespace imCore

#endif // TEXTURE2D_H
