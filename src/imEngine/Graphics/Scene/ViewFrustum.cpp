#include "ViewFrustum.h"
#include <glm/gtx/transform.hpp>

namespace imEngine {


Frustum::Frustum() :
        m_isOrthographic(false),
        m_nearDistance(0.05f),
        m_farDistance(100.0f),
        m_aspectRatio(1.0),
        m_fieldOfView(60.0f),
        m_isNeedToUpdateMatrix(true)
{ }

void Frustum::setOrthographic(bool isOrthographic) {
        m_isOrthographic = isOrthographic;
        m_isNeedToUpdateMatrix = true;
}

bool Frustum::isOrthographic() const {
        return m_isOrthographic;
}

void Frustum::setNearDistance(float distance) {
        m_nearDistance = distance;
        m_isNeedToUpdateMatrix = true;
}

float Frustum::nearDistance() const {
        return m_nearDistance;
}

void Frustum::setFarDistance(float distance) {
        m_farDistance = distance;
        m_isNeedToUpdateMatrix = true;
}

float Frustum::farDistance() const {
        return m_farDistance;
}

void Frustum::setFieldOfView(float angle) {
        m_fieldOfView = angle;
        m_isNeedToUpdateMatrix = true;
}

float Frustum::fieldOfView() const {
        return m_fieldOfView;
}

void Frustum::setMatrix(const Mat4 &matrix) {
}

const Plane& Frustum::plane(FrustumSide side) const {

}

bool Frustum::isInside(Vec3 &vsPoint) {

}

bool Frustum::isInside(Vec3 &vsOrigin, float radius) {

}

const Mat4& Frustum::viewToClipMatrix() const {

}


} //namespace imEngine
