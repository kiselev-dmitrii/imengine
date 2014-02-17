#ifndef FIRSTPERSONCAMERA_H
#define FIRSTPERSONCAMERA_H

#include "Camera.h"

namespace imEngine {


/** @brief Свободная камера
 */
class FirstPersonCamera : public Camera {
public:
        /// Конструктор по умолчанию
        FirstPersonCamera(Object* parent);
        /// Конструктор. Устанавливает начальную позицию
        FirstPersonCamera(const Vec3& psPosition, const Vec3& psUp, const Vec3& psRight, Object* parent);

        /// Перемещает камеру вперед/вправо/вверх
        void            moveForward(float delta);
        void            moveRight(float delta);
        void            moveUp(float delta);

        /// Вращает камеру горизонтально/вертикально на angle
        void            rotateHorizontaly(float angle);
        void            rotateVerticaly(float angle);

        /// Обновление позиции и ориентации камеры
        void            update(float deltaTime);

private:
        void            updatePosition(float deltaTime);
        void            updateOrientation(float deltaTime);
};


} //namespace imEngine

#endif // FIRSTPERSONCAMERA_H
