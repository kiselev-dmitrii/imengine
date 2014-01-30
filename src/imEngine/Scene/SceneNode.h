#ifndef SCENENODE_H
#define SCENENODE_H

#include <imEngine/Math/Common.h>
#include <imEngine/Utils/TreeNode.h>

namespace imEngine {


/** @brief Виды относительных пространств
 */
enum class Space {
        LOCAL,
        PARENT,
        WORLD
};

/** @brief Структура для хранения позиции, поворота и масштаба
 */
struct Transform {
        Vec3    position        = Vec3(0,0,0);          ///< позиция
        Quat    orientation     = Quat(1,0,0,0);        ///< кватернион для хранения поворота
        Vec3    scale           = Vec3(1,1,1);          ///< масштаб по осям
};


/** @brief Иерархическая система координат
 */
class SceneNode : public TreeNode {
public:
        /// Конструктор
        explicit SceneNode(SceneNode* parent = nullptr);
        /// Конструктор. Принимает на вход начальную позицию
        explicit SceneNode(const Vec3& position, const Quat& orientation, const Vec3& scale, SceneNode* parent = nullptr);

        /// Устанавливает/возвращает позицию ноды в пространстве space
        void            setPosition(const Vec3& position, Space space = Space::PARENT);
        const Vec3&     position(Space space = Space::PARENT) const;
        /// Устанавливает/возвращает ориентацию ноды в пространстве space
        void            setOrientation(const Quat& orientation, Space space = Space::PARENT);
        const Quat&     orientation(Space::Enum space = Space::PARENT) const;
        /// Устанавливает/возвращает масштаб ноды по осям в пространстве space
        void            setScale(const Vec3& scale, Space space = Space::PARENT);
        const Vec3&     scale(Space space = Space::PARENT) const;

        /// Перемещает ноду на вектор delta, заданном в пространстве space
        void            translate(const Vec3& delta, Space space = Space::PARENT);
        /// Вращает ноду по кватерниону/по оси заданной в пространстве space
        void            rotate(const Quat& quat, Space space = Space::PARENT);
        void            rotate(const Vec3& axis, float angle, Space space = Space::PARENT);
        // Устанавливает ориентацию таким образом, чтобы ось -Z была направлена в target, а Y в сторону up
        void            lookAt(const Vec3& wsTarget, const Vec3& psUp);

        // Конвертирует вектор/кватернион из мировой СК в локальную
        Vec3            convertWorldToLocal(const Vec3& wsVec) const;
        Quat            convertWorldToLocal(const Quat& wsQuat) const;
        // Конвертирует вектор/кватернион из локальной СК в мировую
        Vec3            convertLocalToWorld(const Vec3& lsVec) const;
        Quat            convertLocalToWorld(const Quat& lsQuat) const;

        /// Возвращает матрицу перехода от локальной СК к мировой СК
        const Mat4&     localToWorldMatrix() const;
        /// Возвращает матрицу перехода от мировой СК к локальной СК
        const Mat4&     worldToLocalMatrix() const;

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
        Transform       m_transform;                            // трансформация в род. системе

        Transform       m_worldTransform;                       // трансформация в мировой системе
        bool            m_isNeedToUpdateWorldTransform;

        Mat4            m_localToWorldMatrix;                   // LS -> WS
        bool            m_isNeedToUpdateLocalToWorldMatrix;

        Mat4            m_worldToLocalMatrix;                   // WS -> LS
        bool            m_isNeedToUpdateWorldToLocalMatrix;
};

} //namespace imEngine

#endif // SCENENODE_H
