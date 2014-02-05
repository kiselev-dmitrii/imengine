#include "Frustum.h"
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

void Frustum::setAspectRatio(float value) {
        m_aspectRatio = value;
        m_isNeedToUpdateMatrix = true;
}

float Frustum::aspectRatio() const {
        return m_aspectRatio;
}

void Frustum::setFieldOfView(float angle) {
        m_fieldOfView = angle;
        m_isNeedToUpdateMatrix = true;
}

float Frustum::fieldOfView() const {
        return m_fieldOfView;
}

const Mat4& Frustum::viewToClipMatrix() {
        updateViewToClipMatrix();
        return m_viewToClipMatrix;
}

void Frustum::updateViewToClipMatrix() {
        if (!m_isNeedToUpdateMatrix) return;

        if (m_isOrthographic) {
                float h = 2 * m_nearDistance * glm::tan(glm::radians(m_fieldOfView)/2);
                float w = h * aspectRatio();
                m_viewToClipMatrix =  glm::ortho(-w/2, w/2, -h/2, h/2);
        } else {
                m_viewToClipMatrix = glm::perspective(m_fieldOfView, m_aspectRatio, m_nearDistance, m_farDistance) ;
        }

        m_isNeedToUpdateMatrix = false;
}


} //namespace imEngine
