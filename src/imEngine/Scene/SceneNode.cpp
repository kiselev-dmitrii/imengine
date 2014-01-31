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
        setPositionInParent(psPosition);
        setOrientationInParent(psOrientation);
        setScaleInParent(psScale);

        notifyTransformUpdated();
}

void SceneNode::setPositionInParent(const Vec3 &psPosition) {
        m_psTransform.position = psPosition;
        notifyTransformUpdated();
}

void SceneNode::setPositionInWorld(const Vec3 &wsPosition) {
        setPositionInParent(convertWorldToParent(wsPosition));
}

const Vec3& SceneNode::positionInParent() {
        return m_psTransform.position;
}

const Vec3& SceneNode::positionInWorld() {
        updateWorldTransform();
        return m_wsTransform.position;
}

void SceneNode::setOrientationInParent(const Quat &psOrientation) {
        m_psTransform.orientation = psOrientation;
        notifyTransformUpdated();
}

void SceneNode::setOrientationInWorld(const Quat &wsOrientation) {
        setOrientationInParent(convertWorldToParent(wsOrientation));
}

const Quat& SceneNode::orientationInParent() {
        return m_psTransform.orientation;
}

const Quat& SceneNode::orientationInWorld() {
        updateWorldTransform();
        return m_wsTransform.orientation;
}

void SceneNode::setScaleInParent(const Vec3 &psScale) {
        m_psTransform.scale = psScale;
        notifyTransformUpdated();
}

void SceneNode::setScaleInWorld(const Vec3 &wsScale) {
        setScaleInParent(convertWorldToParent(wsScale));
}

const Vec3& SceneNode::scaleInParent() {
        return m_psTransform.scale;
}

const Vec3& SceneNode::scaleInWorld() {
        updateWorldTransform();
        return m_wsTransform.scale;
}

void SceneNode::translateInLocal(const Vec3 &lsDelta) {
        m_psTransform.position += m_psTransform.orientation * lsDelta;
        notifyTransformUpdated();
}

void SceneNode::translateInParent(const Vec3 &psDelta) {
        m_psTransform.position += psDelta;
        notifyTransformUpdated();
}

void SceneNode::translateInWorld(const Vec3 &wsDelta) {
        SceneNode* p = (SceneNode*)m_parent;
        if (p) {
                /// Это стоит вынести в отдельную функцию convertWorldToLocalVector
                Vec3 psDelta = (glm::inverse(p->orientationInWorld()) * wsDelta) / p->scaleInWorld();
                translateInParent(psDelta);
        } else {
                translateInParent(wsDelta);
        }
}

void SceneNode::rotateInLocal(const Quat &lsQuat) {
        m_psTransform.orientation = m_psTransform.orientation * glm::normalize(lsQuat);
        notifyTransformUpdated();
}

void SceneNode::rotateInParent(const Quat &psQuat) {
        m_psTransform.orientation = glm::normalize(psQuat) * m_psTransform.orientation;
        notifyTransformUpdated();
}

void SceneNode::rotateInWorld(const Quat &wsQuat) {
        m_psTransform.orientation = m_psTransform.orientation * glm::inverse(orientationInWorld()) *
                                    glm::normalize(wsQuat) * orientationInWorld();
        notifyTransformUpdated();
}

void SceneNode::rotateInLocal(const Vec3 &lsAxis, float angle) {
        Quat lsQuat = glm::angleAxis(angle, lsAxis);
        rotateInLocal(lsQuat);
}

void SceneNode::rotateInParent(const Vec3 &psAxis, float angle) {
        Quat psQuat = glm::angleAxis(angle, psAxis);
        rotateInParent(psQuat);

}

void SceneNode::rotateInWorld(const Vec3 &wsAxis, float angle) {
        Quat wsQuat = glm::angleAxis(angle, wsAxis);
        rotateInWorld(wsQuat);
}

void SceneNode::lookAt(const Vec3 &wsTarget, const Vec3 &psUp) {
        Vec3 psTarget = convertWorldToParent(wsTarget);

        Vec3 psZAxis = -glm::normalize(psTarget - positionInParent());
        Vec3 psXAxis = glm::normalize(glm::cross(psUp, psZAxis));
        Vec3 psYAxis = glm::normalize(glm::cross(psZAxis, psXAxis));

        Mat3 matrix = Mat3(psXAxis.x, psXAxis.y, psXAxis.z,
                           psYAxis.x, psYAxis.y, psYAxis.z,
                           psZAxis.x, psZAxis.y, psZAxis.z);
        setOrientationInParent(glm::toQuat(matrix));
}

Vec3 SceneNode::convertWorldToLocal(const Vec3 &wsVec) {
        return glm::inverse(orientationInWorld())*(wsVec - positionInWorld()) / scaleInWorld();
}

Quat SceneNode::convertWorldToLocal(const Quat &wsQuat) {
        return glm::inverse(orientationInWorld()) * wsQuat;
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
        return (orientationInWorld() *  (lsVec * scaleInWorld())) + positionInWorld();
}

Quat SceneNode::convertLocalToWorld(const Quat &lsQuat) {
        return orientationInWorld() * lsQuat;
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
                Vec3 wsParentPos = p->positionInWorld();
                Quat wsParentOrient = p->orientationInWorld();
                Vec3 wsParentScale = p->scaleInWorld();

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

        Vec3 wsPosition = positionInWorld();
        Quat wsOrientation = orientationInWorld();
        Vec3 wsScale = scaleInWorld();

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
