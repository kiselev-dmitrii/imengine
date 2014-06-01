#ifndef OBJECT_H
#define OBJECT_H

#include <imEngine/Math/Common.h>
#include <imEngine/Math/AABB.h>
#include <imEngine/Utils/TreeNamedNode.h>
#include <imEngine/FileContainers/Json.h>

namespace imEngine {
class Scene;

/** @brief Определяет тип объекта
 */
namespace ObjectType {
        enum Enum {
                OBJECT,
                MOVABLE,
                LIGHT,
                CAMERA,

                FIRST_PERSON_CAMERA,
                TARGET_CAMERA,
                ENTITY,
                VOLUME,
                SPOT_LIGHT,
                POINT_LIGHT
        };

        String   toString(ObjectType::Enum type);
}

/** @brief Структура для хранения позиции, поворота и масштаба
 */
struct Transform {
        Transform() : position(0), orientation(1,0,0,0), scale(1)               { }

        Vec3    position;               ///< позиция
        Quat    orientation;            ///< кватернион для хранения поворота
        Vec3    scale;                  ///< масштаб по осям
};


/** @brief Объект на сцене
 */
class Object : public TreeNamedNode<Object> {
public:
        /// Конструктор
        explicit Object(Object* parent);

        /// Загружает настройки из json-ноды
        virtual void    loadFromJson(const JsonValue& node);
        /// Сохраняет настройки в виде json-ноды
        virtual void    saveAsJson(JsonValue& result);

        /// Возвращает указатель на сцену
        Scene*          scene() const;
        /// Возвращает AABB
        const AABB&     aabb() const;
        /// Возвращает тип объекта
        ObjectType::Enum type() const;

        /// Возвращает позицию/ориентацию/масштаб объекта в родительской СК
        const Vec3&     position() const;
        const Quat&     orientation() const;
        const Vec3&     scale() const;
        /// Возвращает позицию/ориентацию/масштаб объекта в мировой СК
        const Vec3&     worldPosition() const;
        const Quat&     worldOrientation() const;
        const Vec3&     worldScale() const;

        /// Конвертирует вектор/кватернион из мировой СК в локальную
        Vec3            convertWorldToLocal(const Vec3& wsVec) const;
        Quat            convertWorldToLocal(const Quat& wsQuat) const;
        /// Конвертирует вектор/кватернион из мировой СК в родительскую
        Vec3            convertWorldToParent(const Vec3& wsVec) const;
        Quat            convertWorldToParent(const Quat& wsQuat) const;
        /// Конвертирует вектор/кватернион из локальной СК в мировую
        Vec3            convertLocalToWorld(const Vec3& lsVec) const;
        Quat            convertLocalToWorld(const Quat& lsQuat) const;
        /// Возвращает матрицу перехода от локальной СК к мировой СК
        const Mat4&     localToWorldMatrix() const;
        /// Возвращает матрицу перехода от мировой СК к локальной СК
        const Mat4&     worldToLocalMatrix() const;

protected:
        /// Обновляют кэшированные значения, когда это необходимо
        /// (логически и физически они не const, но иначе их не вызвать из const-методов)
        void            updateWorldTransform() const;
        void            updateLocalToWorldMatrix() const;
        void            updateWorldToLocalMatrix() const;
        /// Оповещает объект и дочерние объекты, о том, что позиция данного объекта изменилась
        void            notifyTransformUpdated();

        /// Создает объект по json-ноде
        Object*         createObject(const JsonValue& node);
        Object*         createEntity(const JsonValue& node);
        Object*         createVolume(const JsonValue& node);

protected:
        Scene*                  m_scene;
        AABB                    m_aabb;
        ObjectType::Enum        m_type;

        Transform               m_psTransform;                          // трансформация в род. системе
        /// Кэшированная мировая трансформация
        mutable Transform       m_wsTransform;                        // трансформация в мировой системе
        mutable bool            m_isNeedToUpdateWorldTransform;
        /// Кешированные матрицы
        mutable Mat4            m_localToWorldMatrix;                   // LS -> WS
        mutable Mat4            m_worldToLocalMatrix;                   // WS -> LS
        mutable bool            m_isNeedToUpdateLocalToWorldMatrix;
        mutable bool            m_isNeedToUpdateWorldToLocalMatrix;

};


} //namespace imEngine

#endif // OBJECT_H
