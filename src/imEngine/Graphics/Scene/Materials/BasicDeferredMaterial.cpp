#include "BasicDeferredMaterial.h"
#include <imEngine/Utils/Debug.h>
#include <imEngine/Utils/StringUtils.h>

namespace imEngine {


BasicDeferredMaterial::BasicDeferredMaterial() :
        Material("materials/BasicDeferredMaterial.glsl", false),
        m_diffuseColor(0.7),
        m_specularLevel(1.0),
        m_specularExponent(1.0),
        m_metallicLevel(1.0)
{}

void BasicDeferredMaterial::loadFromXML(const XmlNode &node) {
        String diffuseColor = node.attribute("diffuse_color").value();
        String specularLevel = node.attribute("specular_level").value();
        String specularExponent = node.attribute("specular_exponent").value();
        String metallicLevel = node.attribute("metallic_level").value();

        if (diffuseColor != "") setDiffuseColor(StringUtils::toColor3(diffuseColor));
        if (specularLevel != "") setSpecularLevel(std::stof(specularLevel));
        if (specularExponent != "") setSpecularExponent(std::stof(specularExponent));
        if (metallicLevel != "") setMetallicLevel(std::stof(metallicLevel));
}

void BasicDeferredMaterial::bind() {
        m_program->bind();

        m_program->setUniform("uDiffuseColor", m_diffuseColor);
        m_program->setUniform("uSpecularLevel", m_specularLevel);
        m_program->setUniform("uSpecularExponent", m_specularExponent);
        m_program->setUniform("uMetallicLevel", m_metallicLevel);
}

void BasicDeferredMaterial::unbind() {
        m_program->unbind();
}


} //namespace imEngine
