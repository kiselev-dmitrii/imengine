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

        setScale(Vec2(1.0));

        setDiffuseTexture("model/empty_diffuse.png");
        setSpecularTexture("model/empty_specular.png");
        setNormalTexture("model/empty_normal.png");
}

void GenericMaterial::loadFromJson(const JsonValue &value) {
        JsonValue ambientColor = value["ambientColor"];
        JsonValue diffuseColor = value["diffuseColor"];
        JsonValue specularColor = value["specularColor"];
        JsonValue specularPower = value["specularPower"];

        JsonValue diffuseMap = value["diffuseMap"];
        JsonValue specularMap = value["specularMap"];
        JsonValue normalMap = value["normalMap"];

        JsonValue scale = value["scale"];

        if (!ambientColor.isNull()) setAmbientColor(JsonUtils::toVec3(ambientColor));
        if (!diffuseColor.isNull()) setDiffuseColor(JsonUtils::toVec3(diffuseColor));
        if (!specularColor.isNull()) setSpecularColor(JsonUtils::toVec3(specularColor));
        if (!specularPower.isNull()) setSpecularPower(specularPower.asFloat());

        if (!scale.isNull()) setScale(JsonUtils::toVec2(scale));

        if (!diffuseMap.isNull()) setDiffuseTexture("model/" + diffuseMap.asString());
        if (!specularMap.isNull()) setSpecularTexture("model/" + specularMap.asString());
        if (!normalMap.isNull()) setNormalTexture("model/" + normalMap.asString());
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

        m_program->setUniform("uScale", m_scale);

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
