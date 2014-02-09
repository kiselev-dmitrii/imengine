#ifndef VOLUME_H
#define VOLUME_H

#include "Entity.h"
#include "../Geometry.h"

namespace imEngine {

/** @brief Класс представляет собой отбражаемую трехмерную текстуру.
 *
 * При отображении цвет пикселя будет складываться из суммы цветов текселей
 * текстуры лежащих на одном луче. Объекты данного класса можно использовать
 * в научных целях для визуализации данных (поток газа или жидкости)
 */
class Volume : public Entity {
public:
        /// Конструктор.
        Volume(Texture3D* texture, SceneObject* parent);

        /// Устанавливает/возвращает текстуру с данными
        void            setTexture(Texture3D* texture);
        Texture3D*      texture();

        /// Возвращает программу с котрой происходит рендер
        ProgramPtr      program()                                               { return s_program; }

        /// Рендерит объем
        void            render();


private:
        /// Инициализирует GPU программу
        void            initProgram();

private:
        GeometryPtr             m_cube;
        Texture3D*              m_texture;

        static ProgramPtr       s_program;              ///< общая программа
};

} //namespace imEngine

#endif // VOLUME_H
