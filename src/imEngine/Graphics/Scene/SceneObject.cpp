#include "SceneObject.h"
#include <glm/gtx/quaternion.hpp>

namespace imEngine {

SceneObject::SceneObject(SceneObject *parent) :
        TreeNode(parent),
        m_scene(nullptr),
        m_isNeedToUpdateWorldTransform(true),
        m_isNeedToUpdateLocalToWorldMatrix(true),
        m_isNeedToUpdateWorldToLocalMatrix(true)
{
        if (parent) m_scene = parent->scene();
        notifyTransformUpdated();
}

SceneObject::SceneObject(const Vec3 &psPosition, const Quat &psOrientation, const Vec3 &psScale,
                         SceneObject *parent) :
        TreeNode(parent),
        m_scene(nullptr),
        m_isNeedToUpdateWorldTransform(true),
        m_isNeedToUpdateLocalToWorldMatrix(true),
        m_isNeedToUpdateWorldToLocalMatrix(true)
{
        setPosition(psPosition);
        setOrientation(psOrientation);
        setScale(psScale);
        if (parent) m_scene = parent->scene();

        notifyTransformUpdated();
}

void SceneObject::setPosition(const Vec3 &psPosition) {
        m_psTransform.position = psPosition;
        notifyTransformUpdated();
}

void SceneObject::setWorldPosition(const Vec3 &wsPosition) {
        setPosition(convertWorldToParent(wsPosition));
}

const Vec3& SceneObject::position() {
        return m_psTransform.position;
}

const Vec3& SceneObject::worldPosition() {
        updateWorldTransform();
        return m_wsTransform.position;
}

void SceneObject::setOrientation(const Quat &psOrientation) {
        m_psTransform.orientation = psOrientation;
        notifyTransformUpdated();
}

void SceneObject::setOrientation(const Vec3 &psAxisX, const Vec3 &psAxisY) {
        Vec3 xAxis = glm::normalize(psAxisX);
        Vec3 yAxis = glm::normalize(psAxisY);
        Vec3 zAxis = glm::cross(xAxis, yAxis);

        Mat3 matrix = Mat3(xAxis.x, xAxis.y, xAxis.z,
                           yAxis.x, yAxis.y, yAxis.z,
                           zAxis.x, zAxis.y, zAxis.z);
        setOrientation(glm::toQuat(matrix));
}

void SceneObject::setWorldOrientation(const Quat &wsOrientation) {
        setOrientation(convertWorldToParent(wsOrientation));
}

const Quat& SceneObject::orientation() {
        return m_psTransform.orientation;
}

const Quat& SceneObject::worldOrientation() {
        updateWorldTransform();
        return m_wsTransform.orientation;
}

void SceneObject::setScale(const Vec3 &psScale) {
        m_psTransform.scale = psScale;
        notifyTransformUpdated();
}

void SceneObject::setWorldScale(const Vec3 &wsScale) {
        setScale(convertWorldToParent(wsScale));
}

const Vec3& SceneObject::scale() {
        return m_psTransform.scale;
}

const Vec3& SceneObject::worldScale() {
        updateWorldTransform();
        return m_wsTransform.scale;
}

void SceneObject::translate(const Vec3 &delta, Space space) {
        SceneObject* p = (SceneObject*)m_parent;

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

void SceneObject::rotate(const Quat &quat, Space space) {
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

void SceneObject::rotate(const Vec3 &axis, float angle, Space space) {
        rotate(glm::angleAxis(angle, axis), space);
}


void SceneObject::lookAt(const Vec3 &wsTarget, const Vec3 &psUp) {
        Vec3 psTarget = convertWorldToParent(wsTarget);

        Vec3 psZAxis = -glm::normalize(psTarget - position());
        Vec3 psXAxis = glm::normalize(glm::cross(psUp, psZAxis));
        Vec3 psYAxis = glm::normalize(glm::cross(psZAxis, psXAxis));

        Mat3 matrix = Mat3(psXAxis.x, psXAxis.y, psXAxis.z,
                           psYAxis.x, psYAxis.y, psYAxis.z,
                           psZAxis.x, psZAxis.y, psZAxis.z);
        setOrientation(glm::toQuat(matrix));
}

Vec3 SceneObject::convertWorldToLocal(const Vec3 &wsVec) {
        return glm::inverse(worldOrientation())*(wsVec - worldPosition()) / worldScale();
}

Quat SceneObject::convertWorldToLocal(const Quat &wsQuat) {
        return glm::inverse(worldOrientation()) * wsQuat;
}

Vec3 SceneObject::convertWorldToParent(const Vec3 &wsVec) {
        if (m_parent) return ((SceneObject*)m_parent)->convertWorldToLocal(wsVec);
        else return wsVec;
}

Quat SceneObject::convertWorldToParent(const Quat &wsQuat) {
        if (m_parent) return ((SceneObject*)m_parent)->convertWorldToLocal(wsQuat);
        else return wsQuat;
}

Vec3 SceneObject::convertLocalToWorld(const Vec3 &lsVec) {
        return (worldOrientation() *  (lsVec * worldScale())) + worldPosition();
}

Quat SceneObject::convertLocalToWorld(const Quat &lsQuat) {
        return worldOrientation() * lsQuat;
}

const Mat4& SceneObject::localToWorldMatrix() {
        updateLocalToWorldMatrix();
        return m_localToWorldMatrix;
}

const Mat4& SceneObject::worldToLocalMatrix() {
        updateWorldToLocalMatrix();
        return m_worldToLocalMatrix;
}

Scene* SceneObject::scene() {
        return m_scene;
}

void SceneObject::onAttachChild(TreeNode *node) {
        notifyTransformUpdated();
}

void SceneObject::onDetachChild(TreeNode *node) {
        notifyTransformUpdated();
}

void SceneObject::notifyTransformUpdated() {
        m_isNeedToUpdateWorldTransform = true;
        m_isNeedToUpdateLocalToWorldMatrix = true;
        m_isNeedToUpdateWorldToLocalMatrix = true;

        for (TreeNode* node: children()) ((SceneObject*)node)->notifyTransformUpdated();
}

void SceneObject::updateWorldTransform() {
        if (!m_isNeedToUpdateWorldTransform) return;

        SceneObject* p = (SceneObject*)m_parent;
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

void SceneObject::updateLocalToWorldMatrix() {
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

void SceneObject::updateWorldToLocalMatrix() {
        if (!m_isNeedToUpdateWorldToLocalMatrix) return;

        updateLocalToWorldMatrix();
        m_worldToLocalMatrix = glm::inverse(m_localToWorldMatrix);

        m_isNeedToUpdateWorldToLocalMatrix = false;
}


} //namespace imEngine
