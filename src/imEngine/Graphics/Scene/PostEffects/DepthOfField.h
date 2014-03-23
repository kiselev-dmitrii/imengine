#ifndef DEPTHOFFIELD_H
#define DEPTHOFFIELD_H

#include "Pass.h"
#include "PostEffect.h"
#include <imEngine/Graphics/RenderTarget.h>

namespace imEngine {

class DepthBlurPass : public Pass {
public:
        DepthBlurPass() : Pass("posteffects/DepthOfField.glsl")                 { }

        void setInputTexture(Texture2D* texture)                                { m_inputTexture = texture; }
        void setDepthTexture(Texture2D* texture)                                { m_depthTexture = texture; }
        void setDirection(const Vec2& direction)                                { m_direction = direction; }

protected:
        void prepare() const {
                m_inputTexture->bind(0);
                m_depthTexture->bind(1);
                m_program->setUniform("uInputTexture", 0);
                m_program->setUniform("uDepthTexture", 1);
                m_program->setUniform("uNearDistance", 0.05f);
                m_program->setUniform("uFarDistance", 100.0f);
                m_program->setUniform("uMaxRadius", 200);
                m_program->setUniform("uDirection", m_direction);

        }

private:
        Texture2D*      m_inputTexture;
        Texture2D*      m_depthTexture;
        Vec2            m_direction;
};

/** @brief Постэффект для симулирования глубины резкости
 */
class DepthOfField : public PostEffect {
public:
        DepthOfField();

        void            setInputTexture(Texture2D* texture)                                { m_inputTexture = texture; }
        void            setDepthTexture(Texture2D* texture)                                { m_depthTexture = texture; }

        Texture2D*      apply();

private:
        Texture2D*      m_inputTexture;
        Texture2D*      m_depthTexture;
        DepthBlurPass   m_depthblur;
        RenderTarget    m_rt1;
        RenderTarget    m_rt2;
};

} //namespace imEngine

#endif // DEPTHOFFIELD_H
