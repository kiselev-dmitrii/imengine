#include "SceneNode.h"
#include <glm/gtx/quaternion.hpp>

namespace imEngine {

SceneNode::SceneNode(SceneNode *parent) :
        TreeNode(parent),
        m_isNeedToUpdateWorldTransform(true),
        m_isNeedToUpdateLocalToWorldMatrix(true),
        m_isNeedToUpdateWorldToLocalMatrix(true)
{
        notifyTransformUpdated();
}

SceneNode::SceneNode(const Vec3 &psPosition, const Quat &psOrientation, const Vec3 &psScale,
                     SceneNode *parent) :
        TreeNode(parent),
        m_isNeedToUpdateWorldTransform(true),
        m_isNeedToUpdateLocalToWorldMatrix(true),
        m_isNeedToUpdateWorldToLocalMatrix(true)
{
        setPosition(psPosition);
        setOrientation(psOrientation);
        setScale(psScale);

        notifyTransformUpdated();
}

void SceneNode::setPosition(const Vec3 &psPosition) {
        m_psTransform.position = psPosition;
        notifyTransformUpdated();
}

void SceneNode::setWorldPosition(const Vec3 &wsPosition) {
        setPosition(convertWorldToParent(wsPosition));
}

const Vec3& SceneNode::position() {
        return m_psTransform.position;
}

const Vec3& SceneNode::worldPosition() {
        updateWorldTransform();
        return m_wsTransform.position;
}

void SceneNode::setOrientation(const Quat &psOrientation) {
        m_psTransform.orientation = psOrientation;
        notifyTransformUpdated();
}

void SceneNode::setWorldOrientation(const Quat &wsOrientation) {
        setOrientation(convertWorldToParent(wsOrientation));
}

const Quat& SceneNode::orientation() {
        return m_psTransform.orientation;
}

const Quat& SceneNode::worldOrientation() {
        updateWorldTransform();
        return m_wsTransform.orientation;
}

void SceneNode::setScale(const Vec3 &psScale) {
        m_psTransform.scale = psScale;
        notifyTransformUpdated();
}

void SceneNode::setWorldScale(const Vec3 &wsScale) {
        setScale(convertWorldToParent(wsScale));
}

const Vec3& SceneNode::scale() {
        return m_psTransform.scale;
}

const Vec3& SceneNode::worldScale() {
        updateWorldTransform();
        return m_wsTransform.scale;
}

void SceneNode::translate(const Vec3 &delta, Space space) {
        SceneNode* p = (SceneNode*)m_parent;

        switch (space) {
                case Space::LOCAL:
                        m_psTransform.position += m_psTransform.orientation * delta;
                        break;

                case Space::PARENT:
                        m_psTransform.position += delta;
                        break;

                case Space::WORLD:
                default:
                        if (p) m_psTransform.position += (glm::inverse(p->worldOrientation()) * delta) / p->worldScale();
                        else m_psTransform.position += delta;
                        break;

        }

        notifyTransformUpdated();
}

void SceneNode::rotate(const Quat &quat, Space space) {
        switch (space) {
                case Space::LOCAL:
                        m_psTransform.orientation = m_psTransform.orientation * glm::normalize(quat);
                        break;

                case Space::PARENT:
                        m_psTransform.orientation = glm::normalize(quat) * m_psTransform.orientation;
                        break;

                case Space::WORLD:
                default:
                        m_psTransform.orientation = m_psTransform.orientation * glm::inverse(worldOrientation()) *
                                                    glm::normalize(quat) * worldOrientation();
                        break;
        }

        notifyTransformUpdated();
}

void SceneNode::rotate(const Vec3 &axis, float angle, Space space) {
        rotate(glm::angleAxis(angle, axis), space);
}


void SceneNode::lookAt(const Vec3 &wsTarget, const Vec3 &psUp) {
        Vec3 psTarget = convertWorldToParent(wsTarget);

        Vec3 psZAxis = -glm::normalize(psTarget - position());
        Vec3 psXAxis = glm::normalize(glm::cross(psUp, psZAxis));
        Vec3 psYAxis = glm::normalize(glm::cross(psZAxis, psXAxis));

        Mat3 matrix = Mat3(psXAxis.x, psXAxis.y, psXAxis.z,
                           psYAxis.x, psYAxis.y, psYAxis.z,
                           psZAxis.x, psZAxis.y, psZAxis.z);
        setOrientation(glm::toQuat(matrix));
}

Vec3 SceneNode::convertWorldToLocal(const Vec3 &wsVec) {
        return glm::inverse(worldOrientation())*(wsVec - worldPosition()) / worldScale();
}

Quat SceneNode::convertWorldToLocal(const Quat &wsQuat) {
        return glm::inverse(worldOrientation()) * wsQuat;
}

Vec3 SceneNode::convertWorldToParent(const Vec3 &wsVec) {
        if (m_parent) return ((SceneNode*)m_parent)->convertWorldToLocal(wsVec);
        else return wsVec;
}

Quat SceneNode::convertWorldToParent(const Quat &wsQuat) {
        if (m_parent) return ((SceneNode*)m_parent)->convertWorldToLocal(wsQuat);
        else return wsQuat;
}

Vec3 SceneNode::convertLocalToWorld(const Vec3 &lsVec) {
        return (worldOrientation() *  (lsVec * worldScale())) + worldPosition();
}

Quat SceneNode::convertLocalToWorld(const Quat &lsQuat) {
        return worldOrientation() * lsQuat;
}

const Mat4& SceneNode::localToWorldMatrix() {
        updateLocalToWorldMatrix();
        return m_localToWorldMatrix;
}

const Mat4& SceneNode::worldToLocalMatrix() {
        updateWorldToLocalMatrix();
        return m_worldToLocalMatrix;
}

void SceneNode::onAttachChild(TreeNode *node) {
        notifyTransformUpdated();
}

void SceneNode::onDetachChild(TreeNode *node) {
        notifyTransformUpdated();
}

void SceneNode::notifyTransformUpdated() {
        m_isNeedToUpdateWorldTransform = true;
        m_isNeedToUpdateLocalToWorldMatrix = true;
        m_isNeedToUpdateWorldToLocalMatrix = true;

        for (TreeNode* node: children()) ((SceneNode*)node)->notifyTransformUpdated();
}

void SceneNode::updateWorldTransform() {
        if (!m_isNeedToUpdateWorldTransform) return;

        SceneNode* p = (SceneNode*)m_parent;
        if (p) {
                Vec3 wsParentPos = p->worldPosition();
                Quat wsParentOrient = p->worldOrientation();
                Vec3 wsParentScale = p->worldScale();

                m_wsTransform.position = wsParentOrient * (wsParentScale * m_psTransform.position) + wsParentPos;
                m_wsTransform.orientation = wsParentOrient * m_psTransform.orientation;
                m_wsTransform.scale = wsParentScale * m_psTransform.scale;
        } else {
                m_wsTransform = m_psTransform;
        }

        m_isNeedToUpdateWorldTransform = false;
}

void SceneNode::updateLocalToWorldMatrix() {
        if (!m_isNeedToUpdateLocalToWorldMatrix) return;

        Vec3 wsPosition = worldPosition();
        Quat wsOrientation = worldOrientation();
        Vec3 wsScale = worldScale();

        Mat3 m = glm::toMat3(wsOrientation);

        m_localToWorldMatrix = Mat4(
                wsScale.x*m[0][0], wsScale.y*m[0][1], wsScale.z*m[0][2], 0,
                wsScale.x*m[1][0], wsScale.y*m[1][1], wsScale.z*m[1][2], 0,
                wsScale.x*m[2][0], wsScale.y*m[2][1], wsScale.z*m[2][2], 0,
                wsPosition.x,      wsPosition.y,      wsPosition.z,      1  );

        m_isNeedToUpdateLocalToWorldMatrix = false;
}

void SceneNode::updateWorldToLocalMatrix() {
        if (!m_isNeedToUpdateWorldToLocalMatrix) return;

        updateLocalToWorldMatrix();
        m_worldToLocalMatrix = glm::inverse(m_localToWorldMatrix);

        m_isNeedToUpdateWorldToLocalMatrix = false;
}


} //namespace imEngine
