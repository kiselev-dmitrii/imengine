#include "GenericMaterial.h"
#include <imEngine/System/Filesystem.h>

namespace imEngine {

GenericMaterial::GenericMaterial() :
        EntityMaterial("materials/GenericMaterial.glsl", MaterialType::DEFERRED)
{
        setAmbientColor(Vec3(0));
        setDiffuseColor(Vec3(1));
        setSpecularColor(Vec3(1));
        setSpecularPower(40.0f);

        setDiffuseTexture("models/empty/empty_diffuse.png");
        setSpecularTexture("models/empty/empty_specular.png");
        setNormalTexture("models/empty/empty_normal.png");
}

void GenericMaterial::loadFromJson(const JsonValue &value, const String &texturePath) {
        String ambientColor = value.get("ambientColor", "");
        String diffuseColor = value.get("diffuseColor", "");
        String specularColor = value.get("specularColor", "");
        String specularPower = value.get("specularPower", "");

        String diffuseMap = value.get("diffuseMap", "");
        String specularMap = value.get("specualrMap", "");
        String normalMap = value.get("normalMap", "");

        if (!ambientColor.empty()) setAmbientColor(StringUtils::toVec3(ambientColor));
        if (!diffuseColor.empty()) setDiffuseColor(StringUtils::toVec3(diffuseColor));
        if (!specularColor.empty()) setSpecularColor(StringUtils::toVec3(specularColor));
        if (!specularPower.empty()) setSpecularPower(std::stof(specularPower));

        if (!diffuseMap.empty()) setDiffuseTexture(Filesystem::joinPath(texturePath, diffuseMap));
        if (!specularMap.empty()) setSpecularTexture(Filesystem::joinPath(texturePath, specularMap));
        if (!normalMap.empty()) setNormalTexture(Filesystem::joinPath(texturePath, normalMap));
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
