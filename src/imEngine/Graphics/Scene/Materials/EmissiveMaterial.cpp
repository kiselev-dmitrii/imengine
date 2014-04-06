#include "EmissiveMaterial.h"
#include <imEngine/Utils/StringUtils.h>

namespace imEngine {

EmissiveMaterial::EmissiveMaterial() :
        EntityMaterial("materials/EmissiveMaterial.glsl", MaterialType::UNLIGHTENED),
        m_color(1.0),
        m_power(1.0)
{  }

void EmissiveMaterial::loadFromXML(const XmlNode &node) {
        String color = node.attribute("color").value();
        String power = node.attribute("power").value();

        if (!color.empty()) setColor(StringUtils::toColor3(color));
        if (!power.empty()) setPower(std::stof(power));
}

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
