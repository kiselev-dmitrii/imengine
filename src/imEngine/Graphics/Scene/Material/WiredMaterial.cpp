#include "WiredMaterial.h"

namespace imEngine {

WiredMaterial::WiredMaterial() {
        initProgram("resources/shaders/materials/WiredMaterial.glsl");
        setBaseColor(Vec4(0,0,0,0.1));
        setBorderColor(Vec4(0,0,0,0.5));
        setBorderWidth(0.05);
        setScale(1.0);
}

void WiredMaterial::bind() {
        s_program->bind();

        s_program->setUniform("uBorderWidth", m_borderWidth);
        s_program->setUniform("uBorderColor", m_borderColor);
        s_program->setUniform("uBaseColor", m_baseColor);
        s_program->setUniform("uScale", m_scale);
}

void WiredMaterial::unbind() {
        s_program->unbind();
}

} //namespace imEngine
