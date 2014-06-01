#include "RaycastingMaterial.h"

namespace imEngine {

RaycastingMaterial::RaycastingMaterial() :
        VolumeMaterial("materials/RaycastingMaterial.glsl")
{
        setMinDensity(0.0);
        setMaxDensity(1.0);
        setClipPlane(Vec4(1,1,1,0));
}

void RaycastingMaterial::loadFromJson(const JsonValue &node) {
        JsonValue minDensity = node["min_density"];
        JsonValue maxDensity = node["max_density"];
        JsonValue clipPlane = node["clip_plane"] ;

        if (!minDensity.isNull()) setMinDensity(minDensity.asFloat());
        if (!maxDensity.isNull()) setMaxDensity(maxDensity.asFloat());
        if (!clipPlane.isNull()) setClipPlane(JsonUtils::toVec4(clipPlane));

        VolumeMaterial::loadFromJson(node);
}

void RaycastingMaterial::setClipPlane(const Vec4 &plane) {
        m_clipPlane = Vec4(glm::normalize(Vec3(plane.x, plane.y, plane.z)), plane.w);
}

Vec4 RaycastingMaterial::clipPlane() const {
        return m_clipPlane;
}

void RaycastingMaterial::bind() {
        VolumeMaterial::bind();

        m_program->setUniform("uMinDensity", m_minDensity);
        m_program->setUniform("uMaxDensity", m_maxDensity);
        m_program->setUniform("uClipPlane", m_clipPlane);
}

} //namespace imEngine
