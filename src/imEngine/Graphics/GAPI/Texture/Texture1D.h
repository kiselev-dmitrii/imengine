#ifndef TEXTURE1D_H
#define TEXTURE1D_H

#include "Texture.h"
#include <imEngine/Utils/Types.h>
#include <memory>

namespace imEngine {


/** @brief Класс одномерной текстуры.
 *  Наследует основной класс Texture
 *  Имеет методы для загрузки и сохранение в файл
 */
class Texture1D : public Texture {
public:
        /// Конструктор. Создает одномерную текстуру
        Texture1D();

        /// Загружает данные в текстуру из src
        void            load(int width, InternalFormat::Enum internal, SourceType::Enum srcType, SourceFormat::Enum srcFormat, GLvoid* src, uint srcAlignment = 4);
        /// Загружает данные в текстуру из изображения filename
        void            load(const String& filename);
        /// Выделяет область памяти размера width
        void            allocate(int width, InternalFormat::Enum internal, SourceType::Enum srcType, SourceFormat::Enum srcFormat);

        /// Сохраняет данные в виде изображения 1xWidth()
        void            save(const String& filename, bool overwrite = true);
};


/** @brief Умный указатель с подсчетом ссылок для Texture1D
 */
typedef std::shared_ptr<Texture1D> Texture1DPtr;


} //namespace imEngine

#endif // TEXTURE1D_H
