#ifndef MOVABLE_H
#define MOVABLE_H

#include "Object.h"

namespace imEngine {


/** @brief Определяет относительное пространство
 */
enum class Space {
        LOCAL,
        PARENT,
        WORLD
};


/** @brief Объект, способный изменять свою позицию на сцене
 */
class Movable : public Object {
public:
        /// Конструктор
        explicit Movable(Object* parent);
        /// Конструктор. Принимает на вход начальную позицию
        explicit Movable(const Vec3& position, const Quat& orientation, const Vec3& scale, Object* parent);

        /// Загружает настройки из json-ноды
        void            loadFromJson(const JsonValue &node);
        /// Сохраняет настройки в виде json-ноды
        void            saveAsJson(JsonValue &result);

        /// Устанавливает позицию/ориентацию/масштаб в родительской СК
        void            setPosition(const Vec3& psPosition);
        void            setOrientation(const Quat& psOrientation);
        void            setOrientation(const Vec3& psAxisX, const Vec3& psAxisY);
        void            setScale(const Vec3& psScale);
        /// Устанавливает позицию/ориентацию/масштаб в родительской СК
        void            setWorldPosition(const Vec3& wsPosition);
        void            setWorldOrientation(const Quat& wsOrientation);
        void            setWorldScale(const Vec3& wsScale);

        /// Перемещает ноду на вектор
        void            translate(const Vec3& delta, Space space);
        /// Вращает ноду по кватерниону
        void            rotate(const Quat& quat, Space space);
        /// Вращает ноду относительно заданной оси на угол angle
        void            rotate(const Vec3& axis, float angle, Space space);
        // Устанавливает ориентацию таким образом, чтобы ось -Z была направлена в target, а Y в сторону up
        void            lookAt(const Vec3& wsTarget, const Vec3& psUp);

};

} //namespace imEngine

#endif // MOVABLE_H
