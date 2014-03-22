#include "Pass.h"
#include "../ResourceManager.h"
#include <imEngine/Graphics/ScreenAlignedQuad.h>

namespace imEngine {

Pass::Pass(const String &filename) {
        m_program = RESOURCES->programs()->program(filename);
}

void Pass::apply() const {
        static ScreenAlignedQuad quad;

        m_program->bind();
        prepare();
        quad.render();
        m_program->unbind();
}

Program* Pass::program() const {
        return m_program;
}

//########################### BlitCopyPass ###################################//

BlitCopyPass::BlitCopyPass(Texture2D* texture) :
        Pass("passes/BlitCopyPass.glsl"),
        m_texture(texture)
{ }

void BlitCopyPass::prepare() const {
        m_texture->bind(0);
        m_program->setUniform("uTexture", 0);
}

//########################### ThresholdPass ##################################//

ThresholdPass::ThresholdPass(Texture2D *texture) :
        Pass("passes/ThresholdPass.glsl"),
        m_texture(texture),
        m_threshold(0.5)
{  }

void ThresholdPass::prepare() const {
        m_texture->bind(0);
        m_program->setUniform("uTexture", 0);
        m_program->setUniform("uThreshold", m_threshold);
}

//######################## DirectionalBlurPass ###############################//

DirectionalBlurPass::DirectionalBlurPass(Texture2D *texture) :
        Pass("passes/DirectionalBlurPass.glsl")
{
        setTexture(texture);
        setDirection(Vec2(1.0, 0.0));
        setRadius(16);
        setStep(1);
}

void DirectionalBlurPass::setTexture(Texture2D *texture) {
        m_texture = texture;
        m_texture->setWrap(TextureWrapMode::CLAMP_TO_EDGE);
}

void DirectionalBlurPass::setDirection(const Vec2 &direction) {
        m_direction = glm::normalize(direction);
}

void DirectionalBlurPass::setRadius(int radius) {
        m_radius = radius;
}

void DirectionalBlurPass::setStep(int step) {
        m_step = glm::clamp(step, 1, m_radius);
}

void DirectionalBlurPass::prepare() const {
        m_texture->bind(0);
        m_program->setUniform("uTexture", 0);
        m_program->setUniform("uDirection", m_direction);
        m_program->setUniform("uRadius", m_radius);
        m_program->setUniform("uStep", m_step);
}

} //namespace imEngine
