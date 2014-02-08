#include "PhongMaterial.h"

namespace imEngine {

PhongMaterial::PhongMaterial(const Vec3 &diffuseColor) :
        m_ambientColor(0.1, 0.1, 0.1),
        m_diffuseColor(diffuseColor),
        m_specularColor(1.0),
        m_shininess(40.0f)
{
        initProgram("resources/shaders/materials/PhongMaterial.glsl");
}

void PhongMaterial::bind() {
        s_program->bind();
}

} //namespace imEngine
