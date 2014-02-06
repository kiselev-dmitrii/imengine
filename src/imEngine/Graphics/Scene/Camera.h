#ifndef CAMERA_H
#define CAMERA_H

#include "SceneObject.h"
#include "Frustum.h"
#include <imEngine/Graphics/Geometry.h>

namespace imEngine {


/** @brief Абстрактный класс для камеры
 */
class CameraAbstract : public SceneObject, public Frustum {
public:
        /// Конструктор
        CameraAbstract(SceneObject* parent);

        /// Устанавливает скорость перемещения камеры
        void            setMovementSpeed(float speed);
        void            setRotationSpeed(float speed);

        /// Рендерит хелпер в виде камеры
        void            renderHelper();

        /// Обновление позиции и ориентации камеры
        virtual void    update(float deltaTime) = 0;

protected:
        float           m_movementSpeed;
        float           m_rotationSpeed;

        GeometryPtr     m_geometry;
        ProgramPtr      m_program;
};


/** @brief Свободная камера
 */
class FirstPersonCamera : public CameraAbstract {
public:
        /// Конструктор по умолчанию
        FirstPersonCamera(SceneObject* parent);
        /// Конструктор. Устанавливает начальную позицию
        FirstPersonCamera(const Vec3& psPosition, const Vec3& psUp, const Vec3& psRight, SceneObject* parent);

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


/** @brief Камера, постоянно направленная на некоторую цель
 */
class TargetCamera : public CameraAbstract {
public:
        /// Конструктор
        TargetCamera(const Vec3& psPosition, const Vec3& psUp, const SceneObject& target, SceneObject* parent);

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

#endif // CAMERA_H
