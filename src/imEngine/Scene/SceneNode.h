#ifndef SCENENODE_H
#define SCENENODE_H

#include <imEngine/Math/Common.h>
#include <imEngine/Utils/TreeNode.h>

namespace imEngine {


enum class Space {
        LOCAL,
        PARENT,
        WORLD
};

/** @brief Структура для хранения позиции, поворота и масштаба
 */
struct Transform {
        Transform() : position(0), orientation(1,0,0,0), scale(1)               { }

        Vec3    position;               ///< позиция
        Quat    orientation;            ///< кватернион для хранения поворота
        Vec3    scale;                  ///< масштаб по осям
};


/** @brief Иерархическая система координат
 *
 *  Представляет собой удобный интерфейс для задания позиции, ориентации, масштаба.
 */
class SceneNode : public TreeNode {
public:
        /// Конструктор
        explicit SceneNode(SceneNode* parent = nullptr);
        /// Конструктор. Принимает на вход начальную позицию
        explicit SceneNode(const Vec3& psPosition, const Quat& psOrientation, const Vec3& psScale, SceneNode* parent = nullptr);

        /// Устанавливает/возвращает позицию ноды в родительской/мировой СК
        void            setPosition(const Vec3& psPosition);
        void            setWorldPosition(const Vec3& wsPosition);
        const Vec3&     position();
        const Vec3&     worldPosition();
        /// Устанавливает/возвращает ориентацию ноды в родительской/мировой СК
        void            setOrientation(const Quat& psOrientation);
        void            setWorldOrientation(const Quat& wsOrientation);
        const Quat&     orientation();
        const Quat&     worldOrientation();
        /// Устанавливает/возвращает масштаб ноды по осям родительской/мировой СК.
        void            setScale(const Vec3& psScale);
        void            setWorldScale(const Vec3& wsScale);
        const Vec3&     scale();
        const Vec3&     worldScale();

        /// Перемещает ноду на вектор
        void            translate(const Vec3& delta, Space space);
        /// Вращает ноду по кватерниону
        void            rotate(const Quat& quat, Space space);
        /// Вращает ноду относительно заданной оси на угол angle
        void            rotate(const Vec3& axis, float angle, Space space);
        // Устанавливает ориентацию таким образом, чтобы ось -Z была направлена в target, а Y в сторону up
        void            lookAt(const Vec3& wsTarget, const Vec3& psUp);

public:
        // Конвертирует вектор/кватернион из мировой СК в локальную
        Vec3            convertWorldToLocal(const Vec3& wsVec);
        Quat            convertWorldToLocal(const Quat& wsQuat);
        // Конвертирует вектор/кватернион из мировой СК в родительскую
        Vec3            convertWorldToParent(const Vec3& wsVec);
        Quat            convertWorldToParent(const Quat& wsQuat);
        // Конвертирует вектор/кватернион из локальной СК в мировую
        Vec3            convertLocalToWorld(const Vec3& lsVec);
        Quat            convertLocalToWorld(const Quat& lsQuat);

        /// Возвращает матрицу перехода от локальной СК к мировой СК
        const Mat4&     localToWorldMatrix();
        /// Возвращает матрицу перехода от мировой СК к локальной СК
        const Mat4&     worldToLocalMatrix();

protected:
        virtual void    onAttachChild(TreeNode *node);
        virtual void    onDetachChild(TreeNode *node);

private:
        /// Оповещает ноду и дочерние ноды об изменении трансформации
        void            notifyTransformUpdated();
        /// Обновляет мировую трансформацию, если это необходимо
        void            updateWorldTransform();
        /// Обновляет матрицу перехода LS -> WS, если надо
        void            updateLocalToWorldMatrix();
        /// Обновляет матрицу перехода WS -> LS, если надо
        void            updateWorldToLocalMatrix();

private:
        Transform       m_psTransform;                          // трансформация в род. системе
        Transform       m_wsTransform;                          // трансформация в мировой системе
        bool            m_isNeedToUpdateWorldTransform;

        Mat4            m_localToWorldMatrix;                   // LS -> WS
        bool            m_isNeedToUpdateLocalToWorldMatrix;

        Mat4            m_worldToLocalMatrix;                   // WS -> LS
        bool            m_isNeedToUpdateWorldToLocalMatrix;
};

} //namespace imEngine

#endif // SCENENODE_H
