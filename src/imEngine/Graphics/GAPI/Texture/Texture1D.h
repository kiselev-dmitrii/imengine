#ifndef TEXTURE1D_H
#define TEXTURE1D_H

#include "Texture.h"
#include <imEngine/Utils/Types.h>

namespace imEngine {
namespace GAPI {

/** @brief Класс одномерной текстуры.
 *  Наследует основной класс Texture
 *  Имеет методы для загрузки и сохранение в файл
 */
class Texture1D : public Texture {
public:
        /// Создает одномерную текстуру
        void            create();

        /// Загружает данные в текстуру из src
        void            load(int width, TextureInternalFormat::Enum internal, TextureSrcType::Enum srcType, TextureSrcFormat::Enum srcFormat, GLvoid* src);
        /// Загружает данные в текстуру из изображения filename
        void            load(const String& filename);
        /// Выделяет область памяти размера width
        void            allocate(int width, TextureInternalFormat::Enum internal, TextureSrcType::Enum srcType, TextureSrcFormat::Enum srcFormat);

        /// Сохраняет данные в виде изображения 1xWidth()
        void            save(const String& filename, bool overwrite = true);
};

} //namespace GAPI
} //namespace imEngine

#endif // TEXTURE1D_H
