#include "DepthOfField.h"

namespace imEngine {

DepthOfField::DepthOfField() :
        m_inputTexture(nullptr),
        m_depthTexture(nullptr),
        m_rt(Renderer::viewportSize())
{
        m_rt.enableColorBuffer(0, InternalFormat::COLOR_NORM_4_COMP_8_BIT, true);

}

void DepthOfField::process() {
        Renderer::setBlendMode(BlendMode::NONE);

        m_depthblur.setDepthTexture(m_depthTexture);

        m_rt.bind();
                m_depthblur.setInputTexture(m_inputTexture);
                m_depthblur.setDirection(Vec2(1.0, 0.0));
                m_depthblur.apply();
        m_rt.unbind();

        m_depthblur.setInputTexture(m_rt.colorBufferTexture(0).get());
        m_depthblur.setDirection(Vec2(0.0, 1.0));
        m_depthblur.apply();
}

} //namespace imEngine
