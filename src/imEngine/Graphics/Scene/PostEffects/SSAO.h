#ifndef SSAO_H
#define SSAO_H

#include "PostEffect.h"
#include "Pass.h"
#include <imEngine/Graphics/RenderTarget.h>

namespace imEngine {

class OcclusionCalculationPass : public Pass {
public:
        OcclusionCalculationPass();

        void    setInputTexture(Texture2D* texture)                             { m_inputTexture = texture; }
        void    setNormalTexture(Texture2D* texture)                            { m_normalTexture = texture; }
        void    setDepthTexture(Texture2D* texture)                             { m_depthTexture = texture; }
        void    setOcclusionRadius(int radius)                                  { m_radius = radius; }

protected:
        void    prepare() const;

private:
        Texture2D*      m_inputTexture;
        Texture2D*      m_normalTexture;
        Texture2D*      m_depthTexture;
        int             m_radius;
};


/** @brief Постэффект ScreenSpaceAmbientOcclusion
 */
class SSAO : public PostEffect {
public:
        /// Конструктор
        SSAO();

        /// Входные текстуры
        void            setInputTexture(Texture2D* texture)                     { m_inputTexture = texture; }
        void            setNormalTexture(Texture2D* texture)                    { m_occlusionPass.setNormalTexture(texture); }
        void            setDepthTexture(Texture2D* texture)                     { m_occlusionPass.setDepthTexture(texture); }

        void            setOcclusionRadius(int radius)                          { m_occlusionPass.setOcclusionRadius(radius); }

        /// Применяет постэффект
        Texture2D*      apply();

private:
        Texture2D*                      m_inputTexture;
        OcclusionCalculationPass        m_occlusionPass;
        DirectionalBlurPass             m_blurPass;
        RenderTarget                    m_rt;
};

} //namespace imEngine

#endif // SSAO_H
