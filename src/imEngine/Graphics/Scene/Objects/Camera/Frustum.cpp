#include "Frustum.h"
#include <imEngine/Utils/Debug.h>
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

void Frustum::loadFromJson(const JsonValue &node) {
        JsonValue projection = node["projection"];
        JsonValue near = node["near"];
        JsonValue far = node["far"];
        JsonValue aspect = node["aspect"];
        JsonValue fov = node["fov"];

        if (!projection.isNull()) {
                if (projection.asString() == "ORTHOGRAPHIC") setOrthographic(true);
                if (projection.asString() == "PERSPECTIVE") setOrthographic(false);
                else IM_LOG("Unknown type of project");
        }
        if (!near.isNull()) setNearDistance(near.asFloat());
        if (!far.isNull()) setFarDistance(far.asFloat());
        if (!aspect.isNull()) setAspectRatio(aspect.asFloat());
        if (!fov.isNull()) setFieldOfView(fov.asFloat());
}

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
                float base = (m_farDistance-m_nearDistance)/4;
                float h = 2 * base * glm::tan(glm::radians(m_fieldOfView)/2);
                float w = h * aspectRatio();
                m_viewToClipMatrix = glm::ortho(-w/2, w/2, -h/2, h/2, m_nearDistance, m_farDistance);
        } else {
                m_viewToClipMatrix = glm::perspective(m_fieldOfView, m_aspectRatio, m_nearDistance, m_farDistance) ;
        }

        m_isNeedToUpdateMatrix = false;
}


} //namespace imEngine
