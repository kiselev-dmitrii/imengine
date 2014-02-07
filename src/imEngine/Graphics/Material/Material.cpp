#include "Material.h"
#include <imEngine/Utils/Debug.h>

namespace imEngine {

//########################### TextureMaterial ################################//

TextureMaterial::TextureMaterial(Texture2DPtr texture) {
        initProgram("resources/shaders/materials/TextureMaterial.glsl");
        setTexture(texture);
}

void TextureMaterial::setTexture(Texture2DPtr texture) {
        m_textureBase = texture;
        m_textureBase->setMagnificationFilter(TextureMagFilter::LINEAR);
        m_textureBase->setMinimizationFilter(TextureMinFilter::LINEAR);
}

Texture2DPtr TextureMaterial::texture() const {
        return m_textureBase;
}

void TextureMaterial::bind() {
        s_program->bind();

        m_textureBase->bind(0);
        s_program->setUniform("uTextureBase", 0);
}

void TextureMaterial::unbind() {
        s_program->unbind();
        m_textureBase->unbind();
}

//############################ WiredMaterial #################################//

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
