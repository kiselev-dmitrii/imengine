#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include <memory>
#include "Texture.h"
#include <imEngine/Utils/Types.h>

namespace imEngine {


/** @brief Класс двумерной текстуры.
 *  Позволяет загружать и сохранять текстуру в файл.
 */
class Texture2D : public Texture {
public:
        /// Конструктор. Создает двумерную текстуру
        Texture2D();

        /// Загружает данные в текстуру из src
        void            load(int width, int height, InternalFormat::Enum internal, SourceType::Enum srcType, SourceFormat::Enum srcFormat, GLvoid* src,  uint srcAlignment = 4);
        /// Загружает данные в текстуру из зображения filename
        void            load(const String& filename);
        /// Выделяет область пустую область видеопамяти. Предыдущее содержание уничтожается
        void            allocate(int width, int height, InternalFormat::Enum internal, SourceType::Enum srcType, SourceFormat::Enum srcFormat);
        /// Вставляет внутрь текстуры изображение. Тип и формат данных должен быть тот же, что и у текстуры
        void            insert(int startX, int startY, int width, int height, GLvoid* src, uint srcAlignment = 4, int level = 0);
        /// Очищает текстуру
        void            clear();

        /// Сохраняет данные в изображение filename
        void            save(const String& filename, bool overwrite = true);
};


/** @brief Умный указатель с подсчетом ссылок для Texture2D
 */
typedef std::shared_ptr<Texture2D> Texture2DPtr;


} //namespace imEngine

#endif // TEXTURE2D_H
