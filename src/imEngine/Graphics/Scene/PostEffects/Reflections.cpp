#include "Reflections.h"

namespace imEngine {

//########################### ReflectionPass #################################//

ReflectionPass::ReflectionPass() :
        Pass("passes/ReflectionPass.glsl"),
        m_inputTexture(nullptr),
        m_normalTexture(nullptr),
        m_depthTexture(nullptr),
        m_activeCamera(nullptr)
{
        setViewSpaceStep(0.2);
        setMaxNumSamples(100);
        setNumRefinemetns(10);
}

void ReflectionPass::prepare() const {
        m_inputTexture->bind(0);
        m_normalTexture->bind(1);
        m_depthTexture->bind(2);

        m_program->setUniform("uInputTexture", 0);
        m_program->setUniform("uNormalTexture", 1);
        m_program->setUniform("uDepthTexture", 2);

        m_program->setUniform("uAspectRatio", m_activeCamera->aspectRatio());
        m_program->setUniform("uTanHalfFovy", glm::tan(glm::radians(m_activeCamera->fieldOfView()/2)));
        m_program->setUniform("uNearDistance", m_activeCamera->nearDistance());
        m_program->setUniform("uFarDistance", m_activeCamera->farDistance());
        m_program->setUniform("uProjectionMatrix", m_activeCamera->viewToClipMatrix());

        m_program->setUniform("uViewSpaceStep", m_viewSpaceStep);
        m_program->setUniform("uMaxNumSamples", m_maxNumSamples);
        m_program->setUniform("uNumRefinements", m_numRefinements);
}

//############################ Reflections ###################################//

Reflections::Reflections() :
        m_rt(Renderer::viewportSize())
{
        m_rt.enableColorBuffer(0, InternalFormat::COLOR_FLOAT_3_COMP_16_BIT, true);
}

Texture2D* Reflections::apply() {
        Renderer::setBlendMode(BlendMode::NONE);

        m_rt.bind();
                m_reflectionPass.apply();
        m_rt.unbind();

        return m_rt.colorBufferTexture(0).get();
}


} //namespace imEngine
