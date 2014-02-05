#include "Movable.h"
#include <glm/gtx/quaternion.hpp>

namespace imEngine {

Movable::Movable(Movable *parent) :
        TreeNode(parent),
        m_isNeedToUpdateWorldTransform(true),
        m_isNeedToUpdateLocalToWorldMatrix(true),
        m_isNeedToUpdateWorldToLocalMatrix(true)
{
        notifyTransformUpdated();
}

Movable::Movable(const Vec3 &psPosition, const Quat &psOrientation, const Vec3 &psScale,
                     Movable *parent) :
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

void Movable::setPosition(const Vec3 &psPosition) {
        m_psTransform.position = psPosition;
        notifyTransformUpdated();
}

void Movable::setWorldPosition(const Vec3 &wsPosition) {
        setPosition(convertWorldToParent(wsPosition));
}

const Vec3& Movable::position() {
        return m_psTransform.position;
}

const Vec3& Movable::worldPosition() {
        updateWorldTransform();
        return m_wsTransform.position;
}

void Movable::setOrientation(const Quat &psOrientation) {
        m_psTransform.orientation = psOrientation;
        notifyTransformUpdated();
}

void Movable::setOrientation(const Vec3 &psAxisX, const Vec3 &psAxisY) {
        Vec3 xAxis = glm::normalize(psAxisX);
        Vec3 yAxis = glm::normalize(psAxisY);
        Vec3 zAxis = glm::cross(xAxis, yAxis);

        Mat3 matrix = Mat3(xAxis.x, xAxis.y, xAxis.z,
                           yAxis.x, yAxis.y, yAxis.z,
                           zAxis.x, zAxis.y, zAxis.z);
        setOrientation(glm::toQuat(matrix));
}

void Movable::setWorldOrientation(const Quat &wsOrientation) {
        setOrientation(convertWorldToParent(wsOrientation));
}

const Quat& Movable::orientation() {
        return m_psTransform.orientation;
}

const Quat& Movable::worldOrientation() {
        updateWorldTransform();
        return m_wsTransform.orientation;
}

void Movable::setScale(const Vec3 &psScale) {
        m_psTransform.scale = psScale;
        notifyTransformUpdated();
}

void Movable::setWorldScale(const Vec3 &wsScale) {
        setScale(convertWorldToParent(wsScale));
}

const Vec3& Movable::scale() {
        return m_psTransform.scale;
}

const Vec3& Movable::worldScale() {
        updateWorldTransform();
        return m_wsTransform.scale;
}

void Movable::translate(const Vec3 &delta, Space space) {
        Movable* p = (Movable*)m_parent;

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

void Movable::rotate(const Quat &quat, Space space) {
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

void Movable::rotate(const Vec3 &axis, float angle, Space space) {
        rotate(glm::angleAxis(angle, axis), space);
}


void Movable::lookAt(const Vec3 &wsTarget, const Vec3 &psUp) {
        Vec3 psTarget = convertWorldToParent(wsTarget);

        Vec3 psZAxis = -glm::normalize(psTarget - position());
        Vec3 psXAxis = glm::normalize(glm::cross(psUp, psZAxis));
        Vec3 psYAxis = glm::normalize(glm::cross(psZAxis, psXAxis));

        Mat3 matrix = Mat3(psXAxis.x, psXAxis.y, psXAxis.z,
                           psYAxis.x, psYAxis.y, psYAxis.z,
                           psZAxis.x, psZAxis.y, psZAxis.z);
        setOrientation(glm::toQuat(matrix));
}

Vec3 Movable::convertWorldToLocal(const Vec3 &wsVec) {
        return glm::inverse(worldOrientation())*(wsVec - worldPosition()) / worldScale();
}

Quat Movable::convertWorldToLocal(const Quat &wsQuat) {
        return glm::inverse(worldOrientation()) * wsQuat;
}

Vec3 Movable::convertWorldToParent(const Vec3 &wsVec) {
        if (m_parent) return ((Movable*)m_parent)->convertWorldToLocal(wsVec);
        else return wsVec;
}

Quat Movable::convertWorldToParent(const Quat &wsQuat) {
        if (m_parent) return ((Movable*)m_parent)->convertWorldToLocal(wsQuat);
        else return wsQuat;
}

Vec3 Movable::convertLocalToWorld(const Vec3 &lsVec) {
        return (worldOrientation() *  (lsVec * worldScale())) + worldPosition();
}

Quat Movable::convertLocalToWorld(const Quat &lsQuat) {
        return worldOrientation() * lsQuat;
}

const Mat4& Movable::localToWorldMatrix() {
        updateLocalToWorldMatrix();
        return m_localToWorldMatrix;
}

const Mat4& Movable::worldToLocalMatrix() {
        updateWorldToLocalMatrix();
        return m_worldToLocalMatrix;
}

void Movable::onAttachChild(TreeNode *node) {
        notifyTransformUpdated();
}

void Movable::onDetachChild(TreeNode *node) {
        notifyTransformUpdated();
}

void Movable::notifyTransformUpdated() {
        m_isNeedToUpdateWorldTransform = true;
        m_isNeedToUpdateLocalToWorldMatrix = true;
        m_isNeedToUpdateWorldToLocalMatrix = true;

        for (TreeNode* node: children()) ((Movable*)node)->notifyTransformUpdated();
}

void Movable::updateWorldTransform() {
        if (!m_isNeedToUpdateWorldTransform) return;

        Movable* p = (Movable*)m_parent;
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

void Movable::updateLocalToWorldMatrix() {
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

void Movable::updateWorldToLocalMatrix() {
        if (!m_isNeedToUpdateWorldToLocalMatrix) return;

        updateLocalToWorldMatrix();
        m_worldToLocalMatrix = glm::inverse(m_localToWorldMatrix);

        m_isNeedToUpdateWorldToLocalMatrix = false;
}


} //namespace imEngine
