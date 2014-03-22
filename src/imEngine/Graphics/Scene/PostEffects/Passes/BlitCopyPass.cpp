#include "BlitCopyPass.h"

namespace imEngine {

BlitCopyPass::BlitCopyPass(Texture2D* texture) :
        Pass("passes/BlitCopyPass.glsl"),
        m_texture(texture)
{ }

void BlitCopyPass::prepare() const {
        m_texture->bind(0);
        m_program->setUniform("uTexture", 0);
}

} //namespace imEngine
