#ifndef SSAO_H
#define SSAO_H

#include "PostEffect.h"
#include "Pass.h"
#include <imEngine/Graphics/RenderTarget.h>
#include <imEngine/Utils/Types.h>
#include "../Objects/Camera/Camera.h"

namespace imEngine {

typedef std::vector<Vec3> VectorList;

class OcclusionCalculationPass : public Pass {
public:
        OcclusionCalculationPass();

        void    setInputTexture(Texture2D* texture)                             { m_inputTexture = texture; }
        void    setNormalTexture(Texture2D* texture)                            { m_normalTexture = texture; }
        void    setDepthTexture(Texture2D* texture)                             { m_depthTexture = texture; }
        void    setActiveCamera(Camera* camera)                                 { m_camera = camera; }

        void    setOcclusionRadius(int radius)                                  { m_radius = radius; }
        void    setNumberSamples(uint num)                                      { m_numSamples = num; }

protected:
        /// Метод генерирует вектора лежащие внутри единичной полусферы (длины меньше 1)
        void    generateOffsets();
        void    prepare() const;

private:
        Texture2D*      m_inputTexture;
        Texture2D*      m_normalTexture;
        Texture2D*      m_depthTexture;
        Camera*         m_camera;

        int             m_radius;

        const int       m_maxSamples;           //максимальное количетсво сэмплов (в шейдере определен массив такой же длины)
        VectorList      m_offsets;
        int             m_numSamples;
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
