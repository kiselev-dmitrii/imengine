#include "DeferredMaterial.h"
#include "../ResourceManager.h"
#include <imEngine/System/Filesystem.h>
#include <imEngine/Utils/StringUtils.h>

namespace imEngine {

TexturedDeferredMaterial::TexturedDeferredMaterial() :
        EntityMaterial("materials/TexturedDeferredMaterial.glsl", MaterialType::DEFERRED)
{
        setScale(Vec2(1,1));
        setDiffuseTexture("empty.png");
        setSpecularTexture("empty.png");
        setNormalTexture("empty.png");
}

void TexturedDeferredMaterial::loadFromXML(const XmlNode &node) {
        String diffuseTexture = node.attribute("diffuse_texture").value();
        String specularTexture = node.attribute("specular_texture").value();
        String normalTexture = node.attribute("normal_texture").value();
        String scale = node.attribute("scale").value();

        if (!diffuseTexture.empty()) setDiffuseTexture(diffuseTexture);
        if (!specularTexture.empty()) setSpecularTexture(specularTexture);
        if (!normalTexture.empty()) setNormalTexture(normalTexture);
        if (!scale.empty()) setScale(StringUtils::toVec2(scale));
}

void TexturedDeferredMaterial::setScale(const Vec2 &scale) {
        m_scale = scale;
}

void TexturedDeferredMaterial::setNormalTexture(const String &name) {
        m_normalTexture = RESOURCES->textures()->texture2D(Filesystem::joinPath("model", name));
        m_normalTexture->generateMipmaps();
        m_normalTexture->setMinimizationFilter(TextureMinFilter::LINEAR_MIPMAP_LINEAR);
}

void TexturedDeferredMaterial::setDiffuseTexture(const String &name) {
        m_diffuseTexture = RESOURCES->textures()->texture2D(Filesystem::joinPath("model", name));
        m_diffuseTexture->generateMipmaps();
        m_diffuseTexture->setMinimizationFilter(TextureMinFilter::LINEAR_MIPMAP_LINEAR);
}

void TexturedDeferredMaterial::setSpecularTexture(const String &name) {
        m_specularTexture = RESOURCES->textures()->texture2D(Filesystem::joinPath("model", name));
        m_specularTexture->generateMipmaps();
        m_specularTexture->setMinimizationFilter(TextureMinFilter::LINEAR_MIPMAP_LINEAR);
}

void TexturedDeferredMaterial::bind() {
        m_program->bind();

        m_program->setUniform("uScale", m_scale);

        m_diffuseTexture->bind(0);
        m_program->setUniform("uDiffuseTexture", 0);

        m_specularTexture->bind(1);
        m_program->setUniform("uSpecularTexture", 1);

        m_normalTexture->bind(2);
        m_program->setUniform("uNormalTexture", 2);
}

void TexturedDeferredMaterial::unbind() {
        m_program->unbind();
}



} //namespace imEngine
