#ifndef CAMERA_H
#define CAMERA_H

#include "Frustum.h"
#include "../Movable.h"
#include <imEngine/Graphics/Geometry.h>

namespace imEngine {


/** @brief Абстрактный класс для камеры
 */
class Camera : public Movable, public Frustum {
public:
        /// Конструктор
        explicit Camera(Object* parent);

        /// Загружает настройки из json-ноды
        void            loadFromJson(const JsonValue &node);
        /// Сохраняет настройки в виде json-ноды
        void            saveAsJson(JsonValue &result);

        /// Устанавливает/возвращает скорость перемещения камеры
        void            setMovementSpeed(float speed)                           { m_movementSpeed = speed; }
        float           movementSpeed() const                                   { return m_movementSpeed; }
        /// Устанавливает/возвращает скорость поворота камеры
        void            setRotationSpeed(float speed)                           { m_rotationSpeed = speed; }
        float           rotationSpeed() const                                   { return m_rotationSpeed; }

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
