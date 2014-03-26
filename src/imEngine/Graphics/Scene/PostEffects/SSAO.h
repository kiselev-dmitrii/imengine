#ifndef SSAO_H
#define SSAO_H

#include "PostEffect.h"
#include "Pass.h"
#include <imEngine/Graphics/RenderTarget.h>
#include <imEngine/Utils/Types.h>
#include "../Objects/Camera/Camera.h"

namespace imEngine {

typedef std::vector<Vec3> VectorList;

/** @brief Проход для SSAO
 */
class SSAOPass : public Pass {
public:
        SSAOPass();

        /// Обязательные параметры
        void    setInputTexture(Texture2D* texture)                             { m_inputTexture = texture; }
        void    setNormalTexture(Texture2D* texture)                            { m_normalTexture = texture; }
        void    setDepthTexture(Texture2D* texture)                             { m_depthTexture = texture; }
        void    setActiveCamera(Camera* camera)                                 { m_activeCamera = camera; }

        /// Устанавливает радиус тени в писелях
        void    setScreenRadius(float radius)                                   { m_screenRadius = radius; }
        /// Устанавливает радиус тени в VS (независимо от ScreenRadius)
        void    setViewRadius(float radius)                                     { m_viewRadius = radius; }
        /// Устанавливает четкость теней (число от 0 до 1)
        void    setPower(float power)                                           { m_power = glm::clamp(power, 0.0f, 1.0f); }
        /// Устанавилвает количество сэмплов - влияет на качество
        void    setNumberSamples(int samples)                                   { m_numSamples = glm::clamp(samples, 1, 16); }

protected:
        void    prepare() const;

private:
        Texture2D*      m_inputTexture;
        Texture2D*      m_normalTexture;
        Texture2D*      m_depthTexture;
        Camera*         m_activeCamera;

        float           m_screenRadius;
        float           m_viewRadius;
        float           m_power;
        int             m_numSamples;
};


/** @brief Постэффект ScreenSpaceAmbientOcclusion
 */
class SSAO : public PostEffect {
public:
        /// Конструктор
        SSAO();

        /// Входные текстуры
        void            setInputTexture(Texture2D* texture)                     { m_ssaoPass.setInputTexture(texture); }
        void            setNormalTexture(Texture2D* texture)                    { m_ssaoPass.setNormalTexture(texture); }
        void            setDepthTexture(Texture2D* texture)                     { m_ssaoPass.setDepthTexture(texture); }
        void            setActiveCamera(Camera* camera)                         { m_ssaoPass.setActiveCamera(camera); }

        /// Настройки SSAO прохода
        SSAOPass*       ssaoPass()                                              { return &m_ssaoPass; }

        /// Применяет постэффект
        Texture2D*      apply();

private:
        SSAOPass                        m_ssaoPass;
        RenderTarget                    m_rt;
};

} //namespace imEngine

#endif // SSAO_H
