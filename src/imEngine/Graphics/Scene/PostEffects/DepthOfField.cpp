#include "DepthOfField.h"

namespace imEngine {

DepthOfField::DepthOfField() :
        m_inputTexture(nullptr),
        m_depthTexture(nullptr),
        m_rt(Renderer::viewportSize())
{
        m_rt.enableColorBuffer(0, InternalFormat::COLOR_NORM_4_COMP_8_BIT, true);

}

void DepthOfField::apply() {
        Renderer::setBlendMode(BlendMode::NONE);

        m_hpass.setDepthTexture(m_depthTexture);
        m_vpass.setDepthTexture(m_depthTexture);

        m_rt.bind();
                m_hpass.setInputTexture(m_inputTexture);
                m_hpass.apply();
        m_rt.unbind();

        m_vpass.setInputTexture(m_rt.colorBufferTexture(0).get());
        m_vpass.apply();
}

} //namespace imEngine
