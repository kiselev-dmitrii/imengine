#ifndef DEPTHOFFIELD_H
#define DEPTHOFFIELD_H

#include "Pass.h"
#include <imEngine/Graphics/RenderTarget.h>

namespace imEngine {

class DepthBlurPass : public Pass {
public:
        DepthBlurPass(const StringList& defines) : Pass("posteffects/DepthOfField.glsl", defines) { }

        void setInputTexture(Texture2D* texture)                                { m_inputTexture = texture; }
        void setDepthTexture(Texture2D* texture)                                { m_depthTexture = texture; }

protected:
        void prepare() const {
                m_inputTexture->bind(0);
                m_depthTexture->bind(1);
                m_program->setUniform("uInputTexture", 0);
                m_program->setUniform("uDepthTexture", 1);
                m_program->setUniform("uNearDistance", 0.05f);
                m_program->setUniform("uFarDistance", 100.0f);
                m_program->setUniform("uMaxRadius", 200);

        }

private:
        Texture2D*      m_inputTexture;
        Texture2D*      m_depthTexture;
};


class HDepthBlurPass : public DepthBlurPass {
public:
        HDepthBlurPass() : DepthBlurPass({"HPASS"})                             { }
};

class VDepthBlurPass : public DepthBlurPass {
public:
        VDepthBlurPass() : DepthBlurPass({"VPASS"})                             { }
};


/** @brief Постэффект для симулирования глубины резкости
 */
class DepthOfField {
public:
        DepthOfField();

        void setInputTexture(Texture2D* texture)                                { m_inputTexture = texture; }
        void setDepthTexture(Texture2D* texture)                                { m_depthTexture = texture; }

        void apply();

private:
        Texture2D*      m_inputTexture;
        Texture2D*      m_depthTexture;
        HDepthBlurPass  m_hpass;
        VDepthBlurPass  m_vpass;
        RenderTarget    m_rt;
};

} //namespace imEngine

#endif // DEPTHOFFIELD_H
