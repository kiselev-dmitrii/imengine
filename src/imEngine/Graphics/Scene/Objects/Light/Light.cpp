#include "Light.h"
#include "../../Scene.h"
#include "../../ResourceManager.h"

namespace imEngine {

Light::Light(const String& filename, Object* parent) :
        Movable(parent),
        Pass(filename, {"NO_SHADOW_MAPPING"}),
        m_diffuseBuffer(nullptr),
        m_materialBuffer(nullptr),
        m_geometryBuffer(nullptr),
        m_depthBuffer(nullptr),

        m_diffuseColor(1.0),
        m_specularColor(1.0),
        m_attenuation(1.0),
        m_power(1.0)
{
        m_type = ObjectType::LIGHT;
        scene()->registerLight(this);
}

void Light::loadFromJson(const JsonValue &node) {
        JsonValue diffuseColor = node["diffuse_color"];
        JsonValue specularColor = node["specular_color"];
        JsonValue attenuation = node["attenuation"];
        JsonValue power = node["power"];

        if (!diffuseColor.isNull()) setDiffuseColor(JsonUtils::toVec3(diffuseColor));
        if (!specularColor.isNull()) setSpecularColor(JsonUtils::toVec3(specularColor));
        if (!attenuation.isNull()) setAttenuation(attenuation.asFloat());
        if (!power.isNull()) setPower(power.asFloat());

        Movable::loadFromJson(node);
}

void Light::saveAsJson(JsonValue &result) {
        Movable::saveAsJson(result);

        result["diffuse_color"] = JsonUtils::fromVec3(diffuseColor());
        result["specular_color"] = JsonUtils::fromVec3(specularColor());
        result["attenuation"] = attenuation();
        result["power"] = power();
}

void Light::setGBuffers() const {
        m_diffuseBuffer->bind(0);
        m_materialBuffer->bind(1);
        m_geometryBuffer->bind(2);
        m_depthBuffer->bind(3);

        m_program->setUniform("uGBufferDiffuse", 0);
        m_program->setUniform("uGBufferMaterial", 1);
        m_program->setUniform("uGBufferGeometry", 2);
        m_program->setUniform("uGBufferDepth", 3);
}

void Light::setCameraSettings() const {
        Camera* cam = scene()->activeCamera();

        m_program->setUniform("uAspectRatio", cam->aspectRatio());
        m_program->setUniform("uTanHalfFovy", glm::tan(glm::radians(cam->fieldOfView()/2)));
        m_program->setUniform("uNearDistance", cam->nearDistance());
        m_program->setUniform("uFarDistance", cam->farDistance());
}

void Light::setCommonLightSettings() const {
        m_program->setUniform("uLight.diffuse", m_diffuseColor * m_power);
        m_program->setUniform("uLight.specular", m_specularColor * m_power);
        m_program->setUniform("uLight.attenuation", m_attenuation);
}

void Light::setShadowTechnique(ShadowTechniquePtr technique) {
        m_shadowTechnique = technique;
        m_program->setDefines(technique->defines());
        m_program->build();
}

void Light::calculateShadowMap() {
        if (m_shadowTechnique) {
                m_shadowTechnique->updateShadowMap(this, scene()->entities());
        }
}

} //namespace imEngine
