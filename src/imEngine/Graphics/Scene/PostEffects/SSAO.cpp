#include "SSAO.h"

namespace imEngine {

OcclusionCalculationPass::OcclusionCalculationPass() :
        Pass("passes/OcclusionCalculationPass.glsl"),
        m_inputTexture(nullptr),
        m_normalTexture(nullptr),
        m_depthTexture(nullptr)
{
        m_radius = 16;
}

void OcclusionCalculationPass::prepare() const {
        m_inputTexture->bind(0);
        m_normalTexture->bind(1);
        m_depthTexture->bind(2);

        m_program->setUniform("uInputTexture", 0);
        m_program->setUniform("uNormalTexture", 1);
        m_program->setUniform("uDepthTexture", 2);
        m_program->setUniform("uRadius", m_radius);
}

//################################ SSAO ######################################//

SSAO::SSAO() :
        m_rt(Renderer::viewportSize())
{
        m_rt.enableColorBuffer(0, InternalFormat::COLOR_NORM_4_COMP_8_BIT, true);
}

Texture2D* SSAO::apply() {
        Renderer::setBlendMode(BlendMode::NONE);

        m_rt.bind();
                m_occlusionPass.setInputTexture(m_inputTexture);
                m_occlusionPass.apply();
        m_rt.unbind();

        return m_rt.colorBufferTexture(0).get();
}

} //namespace imEngine
