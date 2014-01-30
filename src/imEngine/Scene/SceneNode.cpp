#include "SceneNode.h"

namespace imEngine {

SceneNode::SceneNode(SceneNode *parent) :
        SceneNode(Vec3(0), Quat(1,0,0,0), Vec3(1), parent)
{ }

SceneNode::SceneNode(const Vec3 &psPosition, const Quat &psOrientation, const Vec3 &spScale,
                     SceneNode *parent) :
        TreeNode(parent),
        m_isNeedToUpdateWorldTransform(true),
        m_isNeedToUpdateLocalToWorldMatrix(true),
        m_isNeedToUpdateWorldToLocalMatrix(true)
{
        setPosition(psPosition);
        setOrientation(psOrientation);
        setScale(spScale);

        notifyTransformUpdated();
}

void SceneNode::setPositionInParent(const Vec3 &psPosition) {
        m_transform.position = psPosition;
        notifyTransformUpdated();
}

const Vec3& SceneNode::positionInParent() const {
        return m_transform.position;
}

void SceneNode::setOrientationInParent(const Quat &psOrientation) {
        m_transform.orientation = psOrientation;
        notifyTransformUpdated();
}

const Quat& SceneNode::orientationInParent() const {
        return m_transform.orientation;
}

void SceneNode::setScaleInParent(const Vec3 &psScale) {
        m_transform.scale = psScale;
        notifyTransformUpdated();
}

const Vec3& SceneNode::scaleInParent() const {
        return m_transform.scale;
}

void SceneNode::setPositionInWorld(const Vec3 &wsPosition) {
        if (m_parent) {
                Vec3 psPosition = static_cast<SceneNode*>(m_parent)->convertWorldToLocal(wsPosition);
                setPositionInParent(psPosition);
        } else {
                setPositionInParent(wsPosition);
        }
}

const Vec3& SceneNode::positionInWorld() const {
        updateWorldTransform();
        return m_worldTransform.position;
}

void SceneNode::setOrientationInWorld(const Quat &wsOrientation) {
        if (m_parent) {
                Quat psOrientation = static_cast<SceneNode*>(m_parent)->convertWorldToLocal(wsOrientation);
                setOrientationInParent(psOrientation);
        } else {
                setOrientationInParent(wsOrientation);
        }
}

const Quat& SceneNode::orientationInWorld() const {
        updateWorldTransform();
        return m_worldTransform.orientation;
}

void SceneNode::setScaleInWorld(const Vec3 &wsScale) {
        if (m_parent) {

        }
}

void SceneNode::notifyTransformUpdated() {
        m_isNeedToUpdateWorldTransform = true;
        m_isNeedToUpdateLocalToWorldMatrix = true;
        m_isNeedToUpdateWorldToLocalMatrix = true;

        for (TreeNode* node: children()) ((SceneNode*)node)->notifyTransformUpdated();
}


} //namespace imEngine
