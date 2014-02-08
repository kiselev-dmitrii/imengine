#include "PhongMaterial.h"

namespace imEngine {

PhongMaterial::PhongMaterial() :
        m_ambientColor(0.1, 0.1, 0.1),
        m_diffuseColor(0.5,0.5,0.5),
        m_specularColor(1.0),
        m_shininess(40.0f)
{
        initProgram("resources/shaders/materials/PhongMaterial.glsl");
}

void PhongMaterial::bind() {
        s_program->bind();

        s_program->setUniform("uMaterial.ambientColor", m_ambientColor);
        s_program->setUniform("uMaterial.diffuseColor", m_diffuseColor);
        s_program->setUniform("uMaterial.specularColor", m_specularColor);
        s_program->setUniform("uMaterial.shininess", m_shininess);

        /// Это настройки света и им тут не место
        s_program->setUniform("uLight.vViewSpacePosition", Vec3(10,10,10));
        s_program->setUniform("uLight.color", Vec3(1.0));
}

void PhongMaterial::unbind() {
        s_program->unbind();
}

} //namespace imEngine
