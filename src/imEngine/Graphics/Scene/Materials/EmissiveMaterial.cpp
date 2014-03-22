#include "EmissiveMaterial.h"

namespace imEngine {

EmissiveMaterial::EmissiveMaterial() :
        Material("materials/EmissiveMaterial.glsl", MaterialType::UNLIGHTENED),
        m_color(1.0),
        m_power(1.0)
{  }

void EmissiveMaterial::setColor(const Vec3 &color) {
        m_color = color;
}

void EmissiveMaterial::setPower(float power) {
        m_power = power;
}

void EmissiveMaterial::bind() {
        m_program->bind();
        m_program->setUniform("uIntensity", m_color*m_power);
}

void EmissiveMaterial::unbind() {
        m_program->unbind();
}

} //namespace imEngine
