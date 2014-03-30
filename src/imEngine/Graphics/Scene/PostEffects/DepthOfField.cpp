#include "DepthOfField.h"

namespace imEngine {

//######################### DepthBlurPass ####################################//

DepthBlurPass::DepthBlurPass() :
        Pass("posteffects/DepthOfField.glsl"),
        m_inputTexture(nullptr),
        m_depthTexture(nullptr),
        m_camera(nullptr),
        m_direction(1, 0),

        m_maxNearRadius(100),
        m_maxFarRadius(200),
        m_focusStart(0.1f),
        m_focusEnd(0.2f)
{ }

void DepthBlurPass::setFocus(float start, float end) {
        m_focusStart = glm::clamp(start, 0.0f, 1.0f);
        m_focusEnd = glm::clamp(end, 0.0f, 1.0f);
        if (start > end) m_focusStart = m_focusEnd = (start + end)/2.0f;
}

void DepthBlurPass::prepare() const {
        m_inputTexture->bind(0);
        m_depthTexture->bind(1);
        m_program->setUniform("uInputTexture", 0);
        m_program->setUniform("uDepthTexture", 1);

        m_program->setUniform("uNearDistance", m_camera->nearDistance());
        m_program->setUniform("uFarDistance", m_camera->farDistance());
        m_program->setUniform("uDirection", m_direction);

        m_program->setUniform("uNearRadius", m_maxNearRadius);
        m_program->setUniform("uFarRadius", m_maxFarRadius);
        m_program->setUniform("uFocusStart", m_focusStart);
        m_program->setUniform("uFocusEnd", m_focusEnd);
}

//######################### DepthOfField #####################################//

DepthOfField::DepthOfField() :
        m_inputTexture(nullptr),
        m_rt1(Renderer::viewportSize()),
        m_rt2(Renderer::viewportSize())
{
        m_rt1.enableColorBuffer(0, InternalFormat::COLOR_NORM_4_COMP_8_BIT, true);
        m_rt2.enableColorBuffer(0, InternalFormat::COLOR_NORM_4_COMP_8_BIT, true);
}

Texture2D* DepthOfField::apply() {
        Renderer::setBlendMode(BlendMode::NONE);

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
