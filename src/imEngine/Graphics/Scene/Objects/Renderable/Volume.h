#ifndef VOLUME_H
#define VOLUME_H

#include "../Movable.h"
#include <imEngine/Graphics/GAPI/GAPI.h>
#include <imEngine/Graphics/Geometry.h>

namespace imEngine {

/** @brief Класс представляет собой отбражаемую трехмерную текстуру.
 *
 * При отображении цвет пикселя будет складываться из суммы цветов текселей
 * текстуры лежащих на одном луче. Объекты данного класса можно использовать
 * в научных целях для визуализации данных (поток газа или жидкости)
 */
class Volume : public Movable {
public:
        /// Конструктор.
        Volume(Texture3D* texture, Object* parent);

        /// Устанавливает/возвращает текстуру с данными
        void            setTexture(Texture3D* texture);
        Texture3D*      texture();

        /// Возвращает программу с котрой происходит рендер
        Program*        program();

        /// Рендерит объем
        void            render();

private:
        GeometryPtr             m_cube;
        Texture3D*              m_texture;
        Program*                m_program;
};

} //namespace imEngine

#endif // VOLUME_H
