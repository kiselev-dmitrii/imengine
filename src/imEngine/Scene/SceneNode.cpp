#include "SceneNode.h"

namespace imEngine {

SceneNode::SceneNode(SceneNode *parent) :
        TreeNode(parent),
        m_isNeedToUpdateWorldTransform(true),
        m_isNeedToUpdateLocalToWorldMatrix(true),
        m_isNeedToUpdateWorldToLocalMatrix(true)
{
        notifyTransformUpdated();
}

SceneNode::SceneNode(const Vec3 &position, const Quat &orientation, const Vec3 &scale,
                     SceneNode *parent) :
        TreeNode(parent),
        m_isNeedToUpdateWorldTransform(true),
        m_isNeedToUpdateLocalToWorldMatrix(true),
        m_isNeedToUpdateWorldToLocalMatrix(true)
{
        setPosition(position);
        setOrientation(orientation);
        setScale(scale);

        notifyTransformUpdated();
}

void SceneNode::notifyTransformUpdated() {
        m_isNeedToUpdateWorldTransform = true;
        m_isNeedToUpdateLocalToWorldMatrix = true;
        m_isNeedToUpdateWorldToLocalMatrix = true;

        for (TreeNode* node: children()) ((SceneNode*)node)->notifyTransformUpdated();
}

void SceneNode::setPosition(const Vec3 &position, Space::Enum space) {
        switch (space) {
                case Space::LOCAL:

                        break;

                case Space::PARENT:
                        m_transform.position = position;
                        notifyChilds();
                        break;

                case Space::WORLD:
                        if (m_parent) {

                        }
                        break;

        }
}

} //namespace imEngine
