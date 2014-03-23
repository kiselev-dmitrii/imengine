#include "Bloom.h"

namespace imEngine {

Bloom::Bloom() :
        m_rt1(Renderer::viewportSize()),
        m_rt2(Renderer::viewportSize())
{
        m_rt1.enableColorBuffer(0, InternalFormat::COLOR_NORM_4_COMP_8_BIT, true);
        m_rt2.enableColorBuffer(0, InternalFormat::COLOR_NORM_4_COMP_8_BIT, true);

        m_threshhold.setThreshold(0.9);
        m_blur.setRadius(100);
        m_blur.setStep(3);
}

void Bloom::setTexture(Texture2D *texture) {
        m_texture = texture;
}

void Bloom::process() {
        Renderer::setBlendMode(BlendMode::NONE);

        m_rt1.bind();
                m_threshhold.setTexture(m_texture);
                m_threshhold.setThreshold(0.8);
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

        Renderer::clearBuffers(Buffer::COLOR);
        Renderer::setBlendMode(BlendMode::ADD);
        m_eq.setTexture(m_texture);
        m_eq.apply();
        m_eq.setTexture(m_rt1.colorBufferTexture(0).get());
        m_eq.apply();
}



} //namespace imEngine
