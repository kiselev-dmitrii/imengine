#include "SSAO.h"
#include <glm/gtx/random.hpp>

namespace imEngine {

SSAOPass::SSAOPass() :
        Pass("passes/SSAOPass.glsl"),
        m_inputTexture(nullptr),
        m_normalTexture(nullptr),
        m_depthTexture(nullptr),
        m_activeCamera(nullptr)
{
        setScreenRadius(8);
        setViewRadius(0.4);
        setPower(0.6);
        setNumberSamples(16);
}

void SSAOPass::prepare() const {
        m_inputTexture->bind(0);
        m_normalTexture->bind(1);
        m_depthTexture->bind(2);

        m_program->setUniform("uInputTexture", 0);
        m_program->setUniform("uNormalTexture", 1);
        m_program->setUniform("uDepthTexture", 2);

        m_program->setUniform("uNearDistance", m_activeCamera->nearDistance());
        m_program->setUniform("uFarDistance", m_activeCamera->farDistance());
        m_program->setUniform("uProjectionMatrix", m_activeCamera->viewToClipMatrix());
        m_program->setUniform("uInvProjectionMatrix", glm::inverse(m_activeCamera->viewToClipMatrix()));

        m_program->setUniform("uScreenRadius", m_screenRadius);
        m_program->setUniform("uViewRadius", m_viewRadius);
        m_program->setUniform("uPower", m_power);
        m_program->setUniform("uNumSamples", m_numSamples);
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
                m_ssaoPass.apply();
        m_rt.unbind();

        return m_rt.colorBufferTexture(0).get();
}

} //namespace imEngine
