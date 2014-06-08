#include "Bloom.h"

namespace imEngine {

//################################ UpSamplingPass #####################################//

UpSamplingPass::UpSamplingPass() :
        Pass("passes/UpSamplingPass.glsl"),
        m_texture(nullptr)
{ }

void UpSamplingPass::prepare() const {
        m_texture->bind(0);
        m_program->setUniform("uTexture", 0);
}

//################################ Bloom #####################################//

Bloom::Bloom() :
        m_rt1(Renderer::viewportSize()/2),
        m_rt2(Renderer::viewportSize()/2),
        m_result(Renderer::viewportSize())
{
        m_rt1.enableColorBuffer(0, InternalFormat::COLOR_FLOAT_3_COMP_16_BIT, true);
        m_rt2.enableColorBuffer(0, InternalFormat::COLOR_FLOAT_3_COMP_16_BIT, true);
        m_result.enableColorBuffer(0, InternalFormat::COLOR_FLOAT_3_COMP_16_BIT, true);

        m_threshhold.setThreshold(0.9);
        m_blur.setRadius(100);
        m_blur.setStep(3);
}

void Bloom::setTexture(Texture2D *texture) {
        m_texture = texture;
}

void Bloom::setRadius(int radius) {
        m_blur.setRadius(radius);
}

void Bloom::setStep(int step) {
        m_blur.setStep(step);
}

void Bloom::setThreshold(float threshold) {
        m_threshhold.setThreshold(threshold);
}

Texture2D* Bloom::apply() {
        Renderer::setBlendMode(BlendMode::NONE);

        m_rt1.bind();
                m_threshhold.setTexture(m_texture);
                m_threshhold.apply();
        m_rt1.unbind();

        m_rt2.bind();
                m_blur.setTexture(m_rt1.colorBufferTexture(0).get());
                m_blur.setDirection(Vec2(1.0, 0.0));
                m_blur.apply();
        m_rt2.unbind();

        m_rt1.bind();
                m_blur.setTexture(m_rt2.colorBufferTexture(0).get());
                m_blur.setDirection(Vec2(0.0, 1.0));
                m_blur.apply();
        m_rt1.unbind();

        m_result.bind();
                Renderer::clearBuffers(Buffer::COLOR);
                Renderer::setBlendMode(BlendMode::ADD);
                m_eq.setTexture(m_texture);
                m_eq.apply();
                m_upsampling.setTexture(m_rt1.colorBufferTexture(0).get());
                m_upsampling.apply();
        m_result.unbind();

        return m_result.colorBufferTexture(0).get();
}



} //namespace imEngine
