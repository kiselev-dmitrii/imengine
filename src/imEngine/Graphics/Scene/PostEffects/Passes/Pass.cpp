#include "Pass.h"
#include "../../ResourceManager.h"
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

} //namespace imEngine
