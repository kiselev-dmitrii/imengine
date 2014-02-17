#ifndef CAMERA_H
#define CAMERA_H

#include "Frustum.h"
#include "../Movable.h"
#include "../../Scene.h"
#include <imEngine/Graphics/Geometry.h>

namespace imEngine {


/** @brief Абстрактный класс для камеры
 */
class Camera : public Movable, public Frustum {
public:
        /// Конструктор
        explicit Camera(Object* parent);

        /// Устанавливает скорость перемещения камеры
        void            setMovementSpeed(float speed);
        void            setRotationSpeed(float speed);

        /// Рендерит хелпер в виде камеры
        void            renderHelper();

        /// Обновление позиции и ориентации камеры
        virtual void    update(float deltaTime) = 0;

private:
        void            initHelper();

protected:
        float           m_movementSpeed;
        float           m_rotationSpeed;

        GeometryPtr     m_geometry;
        Program*        m_program;
};


} //namespace imEngine

#endif // CAMERA_H
