#include "GenericMaterial.h"
#include "../ResourceManager.h"
#include <imEngine/System/Filesystem.h>
#include <imEngine/Utils/StringUtils.h>

namespace imEngine {

GenericMaterial::GenericMaterial() :
        EntityMaterial("materials/GenericMaterial.glsl", MaterialType::DEFERRED)
{
        setAmbientColor(Vec3(0));
        setDiffuseColor(Vec3(1));
        setSpecularColor(Vec3(1));
        setSpecularPower(40.0f);

        setDiffuseTexture("model/empty_diffuse.png");
        setSpecularTexture("model/empty_specular.png");
        setNormalTexture("model/empty_normal.png");
}

void GenericMaterial::loadFromJson(const JsonValue &value) {
        String ambientColor = value.get("ambientColor", "").asString();
        String diffuseColor = value.get("diffuseColor", "").asString();
        String specularColor = value.get("specularColor", "").asString();
        float  specularPower = value.get("specularPower", 40).asFloat();

        String diffuseMap = value.get("diffuseMap", "").asString();
        String specularMap = value.get("specualrMap", "").asString();
        String normalMap = value.get("normalMap", "").asString();

        if (!ambientColor.empty()) setAmbientColor(StringUtils::toVec3(ambientColor));
        if (!diffuseColor.empty()) setDiffuseColor(StringUtils::toVec3(diffuseColor));
        if (!specularColor.empty()) setSpecularColor(StringUtils::toVec3(specularColor));
        setSpecularPower(specularPower);

        if (!diffuseMap.empty()) setDiffuseTexture("model/" + diffuseMap);
        if (!specularMap.empty()) setSpecularTexture("model/" + specularMap);
        if (!normalMap.empty()) setNormalTexture("model/" + normalMap);
}

void GenericMaterial::setDiffuseTexture(const String &path) {
        m_diffuseTexture = RESOURCES->textures()->texture2D(path);
        m_diffuseTexture->generateMipmaps();
        m_diffuseTexture->setMinimizationFilter(TextureMinFilter::LINEAR_MIPMAP_LINEAR);
}

void GenericMaterial::setSpecularTexture(const String &path) {
        m_specularTexture = RESOURCES->textures()->texture2D(path);
        m_specularTexture->generateMipmaps();
        m_specularTexture->setMinimizationFilter(TextureMinFilter::LINEAR_MIPMAP_LINEAR);
}

void GenericMaterial::setNormalTexture(const String &path) {
        m_normalTexture = RESOURCES->textures()->texture2D(path);
        m_normalTexture->generateMipmaps();
        m_normalTexture->setMinimizationFilter(TextureMinFilter::LINEAR_MIPMAP_LINEAR);
}

void GenericMaterial::bind() {
        m_program->bind();

        m_program->setUniform("uAmbientColor", m_ambientColor);
        m_program->setUniform("uDiffuseColor", m_diffuseColor);
        m_program->setUniform("uSpecularColor", m_specularColor);
        m_program->setUniform("uSpecularPower", m_specularPower);

        m_diffuseTexture->bind(0);
        m_program->setUniform("uDiffuseTexture", 0);
        m_specularTexture->bind(1);
        m_program->setUniform("uSpecularTexture", 1);
        m_normalTexture->bind(2);
        m_program->setUniform("uNormalTexture", 2);
}

void GenericMaterial::unbind() {
        m_program->unbind();
}

} //namespace imEngine
