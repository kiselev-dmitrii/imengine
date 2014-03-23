#include "DepthOfField.h"

namespace imEngine {

DepthOfField::DepthOfField() :
        m_inputTexture(nullptr),
        m_depthTexture(nullptr),
        m_rt1(Renderer::viewportSize()),
        m_rt2(Renderer::viewportSize())
{
        m_rt1.enableColorBuffer(0, InternalFormat::COLOR_NORM_4_COMP_8_BIT, true);
        m_rt2.enableColorBuffer(0, InternalFormat::COLOR_NORM_4_COMP_8_BIT, true);

}

Texture2D* DepthOfField::apply() {
        Renderer::setBlendMode(BlendMode::NONE);

        m_depthblur.setDepthTexture(m_depthTexture);

        m_rt1.bind();
                m_depthblur.setInputTexture(m_inputTexture);
                m_depthblur.setDirection(Vec2(1.0, 0.0));
                m_depthblur.apply();
        m_rt1.unbind();

        m_rt2.bind();
                m_depthblur.setInputTexture(m_rt1.colorBufferTexture(0).get());
                m_depthblur.setDirection(Vec2(0.0, 1.0));
                m_depthblur.apply();
        m_rt2.unbind();

        return m_rt2.colorBufferTexture(0).get();
}

} //namespace imEngine
