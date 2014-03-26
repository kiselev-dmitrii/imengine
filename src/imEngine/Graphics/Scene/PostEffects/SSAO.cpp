#include "SSAO.h"
#include <glm/gtx/random.hpp>

namespace imEngine {

const int SSAOPass::s_maxSamples = 128;

SSAOPass::SSAOPass() :
        Pass("passes/SSAOPass.glsl"),
        m_normalTexture(nullptr),
        m_depthTexture(nullptr),
        m_activeCamera(nullptr),

        m_radius(0.3),
        m_penumbra(0.1),
        m_numSamples(16),
        m_offsets(s_maxSamples)
{
        generateOffsets();
}

void SSAOPass::generateOffsets() {
        for (int i = 0; i < s_maxSamples; ++i) {
                m_offsets[i] = Vec3(
                        glm::compRand1(-1.0f, 1.0f),
                        glm::compRand1(-1.0f, 1.0f),
                        glm::compRand1(-1.0f, 1.0f)
                );
                m_offsets[i] = glm::normalize(m_offsets[i]);
        }
}

void SSAOPass::prepare() const {
        float nearDistance = m_activeCamera->nearDistance();
        float farDistance = m_activeCamera->farDistance();
        const Mat4& projectionMatrix = m_activeCamera->viewToClipMatrix();
        Mat4 invProjectionMatrix = glm::inverse(projectionMatrix);

        m_inputTexture->bind(0);
        m_normalTexture->bind(1);
        m_depthTexture->bind(2);

        m_program->setUniform("uInputTexture", 0);
        m_program->setUniform("uNormalTexture", 1);
        m_program->setUniform("uDepthTexture", 2);

        m_program->setUniform("uNearDistance", nearDistance);
        m_program->setUniform("uFarDistance", farDistance);
        m_program->setUniform("uProjectionMatrix", projectionMatrix);
        m_program->setUniform("uInvProjectionMatrix", invProjectionMatrix);

        m_program->setUniform("uOffsets[0]", &(m_offsets[0]), s_maxSamples);
        m_program->setUniform("uNumSamples", m_numSamples);

        m_program->setUniform("uRadius", m_radius);
        m_program->setUniform("uPenumbra", m_penumbra);
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
