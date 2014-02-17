#ifndef TARGETCAMERA_H
#define TARGETCAMERA_H

#include "Camera.h"

namespace imEngine {


/** @brief Камера, постоянно направленная на некоторую цель
 */
class TargetCamera : public Camera {
public:
        /// Конструктор
        TargetCamera(const Vec3& psPosition, const Vec3& psUp, const Movable& target, Object* parent);

        /// Перемещает вперед к цели
        void            zoomIn(float delta);
        /// Перемещает камеру вправо на угол delta относительно цели
        void            rotateRight(float deltaDegree);
        /// Перемещает камеру вверх на угол delta относительно цели
        void            rotateUp(float deltaDegree);

        /// Обновление позиции и ориентации камеры
        void            update(float deltaTime);
};


} //namespace imEngine

#endif // TARGETCAMERA_H
