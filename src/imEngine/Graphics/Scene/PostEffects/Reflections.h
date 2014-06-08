#ifndef REFLECTIONS_H
#define REFLECTIONS_H

#include "PostEffect.h"
#include "Pass.h"
#include "../Objects/Camera/Camera.h"
#include <imEngine/Graphics/RenderTarget.h>
#include <imEngine/Utils/Types.h>

namespace imEngine {

/** @brief Проход, в котором рассчитывается отражения путем трассировки лучей
 */
class ReflectionPass : public Pass {
public:
        /// Конструктор
        ReflectionPass();

        /// Обязательные параметры
        void    setInputTexture(Texture2D* texture)                             { m_inputTexture = texture; }
        void    setNormalTexture(Texture2D* texture)                            { m_normalTexture = texture; }
        void    setDepthTexture(Texture2D* texture)                             { m_depthTexture = texture; }
        void    setActiveCamera(Camera* camera)                                 { m_activeCamera = camera; }

        /// Устанавливает/возвращает шаг трассировки в ViewSpace
        void    setViewSpaceStep(float step)                                    { m_viewSpaceStep = glm::clamp(step, 0.0001f, 100.0f); }
        float   viewSpaceStep() const                                           { return m_viewSpaceStep; }
        /// Устанавливает/возвращает максимальное количество сэмплов
        void    setMaxNumSamples(int samples)                                   { m_maxNumSamples = glm::clamp(samples, 0, 200); }
        int     maxNumSamples() const                                           { return m_maxNumSamples; }

protected:
        void    prepare() const;

private:
        Texture2D*      m_inputTexture;
        Texture2D*      m_normalTexture;
        Texture2D*      m_depthTexture;
        Camera*         m_activeCamera;

        float           m_viewSpaceStep;
        int             m_maxNumSamples;
};


/** @brief Постэффект Screen Space Raytracing Reflections
 */
class Reflections : public PostEffect {
public:
        /// Конструктор
        Reflections();

        /// Входные текстуры
        void            setInputTexture(Texture2D* texture)                     { m_reflectionPass.setInputTexture(texture); }
        void            setNormalTexture(Texture2D* texture)                    { m_reflectionPass.setNormalTexture(texture); }
        void            setDepthTexture(Texture2D* texture)                     { m_reflectionPass.setDepthTexture(texture); }
        void            setActiveCamera(Camera* camera)                         { m_reflectionPass.setActiveCamera(camera); }

        /// Настройки прохода
        ReflectionPass* reflectionPass()                                        { return &m_reflectionPass; }

        /// Применяет постэффект
        Texture2D*      apply();

private:
        ReflectionPass  m_reflectionPass;
        RenderTarget    m_rt;
};

} //namespace imEngine

#endif // REFLECTIONS_H
