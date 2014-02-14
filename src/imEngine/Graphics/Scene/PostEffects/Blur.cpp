#include "Blur.h"

namespace imEngine {


HBlur::HBlur() :
        PostEffectImplementation("resources/shaders/posteffects/hblur.glsl"),
        m_texture(nullptr),
        m_sigma(1.0)
{  }

void HBlur::setTexture(Texture2D *texture) {
        m_texture = texture;
}

void HBlur::setSigma(float sigma) {
        m_sigma = sigma;
}

void HBlur::bind() {
        s_program->bind();

        m_texture->bind(0);
        s_program->setUniform("uTexture", 0);
        s_program->setUniform("uSigma", m_sigma);
}

void HBlur::unbind() {
        s_program->unbind();
}


//############################# VBlur ########################################//

VBlur::VBlur() :
        PostEffectImplementation("resources/shaders/posteffects/vblur.glsl"),
        m_texture(nullptr),
        m_sigma(1.0)
{  }

void VBlur::setTexture(Texture2D *texture) {
        m_texture = texture;
}

void VBlur::setSigma(float sigma) {
        m_sigma = sigma;
}

void VBlur::bind() {
        s_program->bind();

        m_texture->bind(0);
        s_program->setUniform("uTexture", 0);
        s_program->setUniform("uSigma", m_sigma);
}

void VBlur::unbind() {
        s_program->unbind();
}


} //namespace imEngine

