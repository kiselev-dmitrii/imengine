#ifndef SSAO_H
#define SSAO_H

#include "PostEffect.h"
#include "Pass.h"
#include <imEngine/Graphics/RenderTarget.h>
#include <imEngine/Utils/Types.h>
#include "../Objects/Camera/Camera.h"

namespace imEngine {

typedef std::vector<Vec3> VectorList;

class SSAOPass : public Pass {
public:
        SSAOPass();

        /// Обязательные параметры
        void    setInputTexture(Texture2D* texture)                             { m_inputTexture = texture; }
        void    setNormalTexture(Texture2D* texture)                            { m_normalTexture = texture; }
        void    setDepthTexture(Texture2D* texture)                             { m_depthTexture = texture; }
        void    setActiveCamera(Camera* camera)                                 { m_activeCamera = camera; }

        /// Установка радиуса теней в VS
        void    setRadius(float radius)                                         { m_radius = radius; }
        /// Установка размера полутени в VS
        void    setPenumbra(float distance)                                     { m_penumbra = glm::clamp(distance, 0.0f, m_radius); }
        /// Установка количества сделанных сэмплов (больше сэмплов - лучше качество)
        void    setNumberSamples(int num)                                       { m_numSamples = glm::clamp(num, 1, s_maxSamples); }

protected:
        /// Генерирует m_maxSpales векторов, равномерно расположенных на единичной сфере
        void    generateOffsets();
        /// Передает в шейдер параметры перед вызовом
        void    prepare() const;

private:
        Texture2D*      m_inputTexture;
        Texture2D*      m_normalTexture;
        Texture2D*      m_depthTexture;
        Camera*         m_activeCamera;

        float           m_radius;
        float           m_penumbra;
        int             m_numSamples;

        static const int        s_maxSamples;
        VectorList              m_offsets;
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
        void            setActiveCamera(Camera* camera)                         { m_occlusionPass.setActiveCamera(camera); }

        void            setOcclusionRadius(int radius)                          { m_occlusionPass.setRadius(radius); }

        /// Применяет постэффект
        Texture2D*      apply();

private:
        Texture2D*                      m_inputTexture;
        SSAOPass        m_occlusionPass;
        DirectionalBlurPass             m_blurPass;
        RenderTarget                    m_rt;
};

} //namespace imEngine

#endif // SSAO_H
