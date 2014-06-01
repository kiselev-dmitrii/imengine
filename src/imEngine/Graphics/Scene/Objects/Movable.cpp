#include "Movable.h"
#include <glm/gtx/quaternion.hpp>

namespace imEngine {

Movable::Movable(Object *parent) :
        Object(parent)
{ }

Movable::Movable(const Vec3 &position, const Quat &orientation, const Vec3 &scale, Object *parent) :
        Object(parent)
{
        m_type = ObjectType::MOVABLE;

        setPosition(position);
        setOrientation(orientation);
        setScale(scale);
}

void Movable::loadFromJson(const JsonValue &node) {
        JsonValue position = node["position"];
        JsonValue orientation = node["orientation"];
        JsonValue scale = node["scale"];

        JsonValue worldPosition = node["world_position"];
        JsonValue worldOrientation = node["world_orientation"];
        JsonValue worldScale = node["world_scale"];

        JsonValue target = node["target"];
        JsonValue up = node["up"];

        if (!position.isNull()) setPosition(JsonUtils::toVec3(position));
        if (!orientation.isNull()) setOrientation(JsonUtils::toQuat(orientation));
        if (!scale.isNull()) setScale(JsonUtils::toVec3(scale));

        if (!worldPosition.isNull()) setWorldPosition(JsonUtils::toVec3(worldPosition));
        if (!worldOrientation.isNull()) setWorldOrientation(JsonUtils::toQuat(worldOrientation));
        if (!worldScale.isNull()) setWorldScale(JsonUtils::toVec3(worldScale));

        if (!target.isNull()) {
                if (!up.isNull()) lookAt(JsonUtils::toVec3(target), JsonUtils::toVec3(up));
                else lookAt(JsonUtils::toVec3(target), Vec3(0,1,0));
        }

        Object::loadFromJson(node);
}

void Movable::saveAsJson(JsonValue &result) {
        Object::saveAsJson(result);

        result["position"] = JsonUtils::fromVec3(position());
        result["orientation"] = JsonUtils::fromQuat(orientation());
        result["scale"] = JsonUtils::fromVec3(scale());
}

void Movable::setPosition(const Vec3 &psPosition) {
        m_psTransform.position = psPosition;
        notifyTransformUpdated();
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

void Movable::setScale(const Vec3 &psScale) {
        m_psTransform.scale = psScale;
        notifyTransformUpdated();
}

void Movable::setWorldPosition(const Vec3 &wsPosition) {
        setPosition(convertWorldToParent(wsPosition));
}

void Movable::setWorldOrientation(const Quat &wsOrientation) {
        setOrientation(convertWorldToParent(wsOrientation));
}

void Movable::setWorldScale(const Vec3 &wsScale) {
        setScale(convertWorldToParent(wsScale));
}

void Movable::translate(const Vec3 &delta, Space space) {
        Object* p = m_parent;

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


} //namespace imEngine
