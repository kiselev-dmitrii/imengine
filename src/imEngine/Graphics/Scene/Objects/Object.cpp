#include "Object.h"
#include <glm/gtx/quaternion.hpp>
#include "../Scene.h"

namespace imEngine {


//############################ Object ########################################//

Object::Object(Object *parent) :
        TreeNamedNode(parent),
        m_scene(nullptr),
        m_isNeedToUpdateWorldTransform(true),
        m_isNeedToUpdateLocalToWorldMatrix(true),
        m_isNeedToUpdateWorldToLocalMatrix(true)
{
        if (parent != nullptr) m_scene = parent->scene();
        notifyTransformUpdated();
}

Scene* Object::scene() const {
        return m_scene;
}

const AABB& Object::aabb() const {
        return m_aabb;
}

const Vec3& Object::position() const {
        return m_psTransform.position;
}

const Quat& Object::orientation() const {
        return m_psTransform.orientation;
}

const Vec3& Object::scale() const {
        return m_psTransform.scale;
}

const Vec3& Object::worldPosition() const {
        updateWorldTransform();
        return m_wsTransform.position;
}

const Quat& Object::worldOrientation() const {
        updateWorldTransform();
        return m_wsTransform.orientation;
}

const Vec3& Object::worldScale() const {
        updateWorldTransform();
        return m_wsTransform.scale;
}

Vec3 Object::convertWorldToLocal(const Vec3 &wsVec) const {
        return glm::inverse(worldOrientation())*(wsVec - worldPosition()) / worldScale();
}

Quat Object::convertWorldToLocal(const Quat &wsQuat) const {
        return glm::inverse(worldOrientation()) * wsQuat;
}

Vec3 Object::convertWorldToParent(const Vec3 &wsVec) const {
        if (m_parent) return m_parent->convertWorldToLocal(wsVec);
        else return wsVec;
}

Quat Object::convertWorldToParent(const Quat &wsQuat) const {
        if (m_parent) return m_parent->convertWorldToLocal(wsQuat);
        else return wsQuat;
}

Vec3 Object::convertLocalToWorld(const Vec3 &lsVec) const {
        return (worldOrientation() *  (lsVec * worldScale())) + worldPosition();
}

Quat Object::convertLocalToWorld(const Quat &lsQuat) const {
        return worldOrientation() * lsQuat;
}

const Mat4& Object::localToWorldMatrix() const {
        updateLocalToWorldMatrix();
        return m_localToWorldMatrix;
}

const Mat4& Object::worldToLocalMatrix() const {
        updateWorldToLocalMatrix();
        return m_worldToLocalMatrix;
}

void Object::updateWorldTransform() const {
        if (!m_isNeedToUpdateWorldTransform) return;

        Object* p = m_parent;
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

void Object::updateLocalToWorldMatrix() const {
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

void Object::updateWorldToLocalMatrix() const {
        if (!m_isNeedToUpdateWorldToLocalMatrix) return;

        updateLocalToWorldMatrix();
        m_worldToLocalMatrix = glm::inverse(m_localToWorldMatrix);

        m_isNeedToUpdateWorldToLocalMatrix = false;
}

void Object::notifyTransformUpdated() {
        m_isNeedToUpdateWorldTransform = true;
        m_isNeedToUpdateLocalToWorldMatrix = true;
        m_isNeedToUpdateWorldToLocalMatrix = true;

        for (Object* node: children()) node->notifyTransformUpdated();
}



} //namespace imEngine
