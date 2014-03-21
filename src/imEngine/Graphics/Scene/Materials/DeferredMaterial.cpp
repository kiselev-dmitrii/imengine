#include "DeferredMaterial.h"
#include "../ResourceManager.h"
#include <imEngine/System/Filesystem.h>

namespace imEngine {

TexturedDeferredMaterial::TexturedDeferredMaterial() :
        Material("materials/TexturedDeferredMaterial.glsl", false)
{
        m_diffuseTexture = m_specularTexture = m_normalTexture = RESOURCES->textures()->texture2D("model/empty.png");
}

void TexturedDeferredMaterial::loadFromXML(const XmlNode &node) {
        String diffuseTexture = node.attribute("diffuse_texture").value();
        String specularTexture = node.attribute("specular_texture").value();
        String normalTexture = node.attribute("normal_texture").value();

        if (!diffuseTexture.empty()) setDiffuseTexture(diffuseTexture);
        if (!specularTexture.empty()) setSpecularTexture(specularTexture);
        if (!normalTexture.empty()) setNormalTexture(normalTexture);
}

void TexturedDeferredMaterial::setNormalTexture(const String &name) {
        m_normalTexture = RESOURCES->textures()->texture2D(Filesystem::joinPath("model", name));
}

void TexturedDeferredMaterial::setDiffuseTexture(const String &name) {
        m_diffuseTexture = RESOURCES->textures()->texture2D(Filesystem::joinPath("model", name));
}

void TexturedDeferredMaterial::setSpecularTexture(const String &name) {
        m_specularTexture = RESOURCES->textures()->texture2D(Filesystem::joinPath("model", name));
}

void TexturedDeferredMaterial::bind() {
        m_program->bind();

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
