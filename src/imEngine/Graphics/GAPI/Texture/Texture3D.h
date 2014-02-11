#ifndef TEXTURE3D_H
#define TEXTURE3D_H

#include <memory>
#include "Texture.h"
#include <imEngine/Utils/Types.h>

namespace imEngine {


/** @brief Класс трехмерной текстуры.
 */
class Texture3D : public Texture {
public:
        /// Конструктор. Создает 3D текстуру
        Texture3D();

        /// Загружает данные в текстуру из src
        void            load(int width, int height, int depth, InternalFormat::Enum internal,
                             SourceType::Enum srcType, SourceFormat::Enum srcFormat, GLvoid* src);
        /// Загружает данные в текстуру из изображений filenames
        void            load(const StringList& filenames);
        /// Загружает сырые данные из файла
        void            load(int width, int height, int depth, InternalFormat::Enum internal,
                             SourceType::Enum srcType, SourceFormat::Enum srcFormat, const String& filename);
        /// Выделяет область пустую область видеопамяти. Предыдущее содержание уничтожается
        void            allocate(int width, int height, int depth, InternalFormat::Enum internal,
                                 SourceType::Enum srcType, SourceFormat::Enum srcFormat);
};


/** @brief Умный указатель с подсчетом ссылок для Texture1D
 */
typedef std::shared_ptr<Texture3D> Texture3DPtr;


} //namespace imEngine

#endif // TEXTURE3D_H
