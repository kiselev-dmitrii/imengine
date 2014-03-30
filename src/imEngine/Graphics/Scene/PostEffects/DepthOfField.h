#ifndef DEPTHOFFIELD_H
#define DEPTHOFFIELD_H

#include "Pass.h"
#include "PostEffect.h"
#include <imEngine/Graphics/RenderTarget.h>
#include "../Objects/Camera/Camera.h"

namespace imEngine {

/** @brief Проход направленного размытия изображения input.
 *
 * Радиус размытия зависит от значения в буфере глубины
 */
class DepthBlurPass : public Pass {
public:
        /// Конструктор
        DepthBlurPass();

        /// Обязательные параметры
        void    setInputTexture(Texture2D* texture)                             { m_inputTexture = texture; }
        void    setDepthTexture(Texture2D* texture)                             { m_depthTexture = texture; }
        void    setActiveCamera(Camera* camera)                                 { m_camera = camera; }

        /// Устанавливает направление размытия
        void    setDirection(const Vec2& direction)                             { m_direction = direction; }

        /// Устанавливает/возвращает ближний максимальный радиус размытия
        void    setMaxNearRadius(int radius)                                    { m_maxNearRadius = radius; }
        int     maxNearRadius() const                                           { return m_maxNearRadius; }
        /// Устанавливает/возвращает максимальный дальний радиус размытия
        void    setMaxFarRadius(int radius)                                     { m_maxFarRadius = radius; }
        int     maxFarRadius() const                                            { return m_maxFarRadius; }
        /// Устанавливает/возвращает дистанцию до начала фокусированной области в [0;1]
        /// start < end
        void    setFocusStart(float start)                                      { m_focusStart = glm::clamp(start, 0.0f, m_focusEnd); }
        float   focusStart() const                                              { return m_focusStart; }
        void    setFocusEnd(float end)                                          { m_focusEnd = glm::clamp(end, m_focusStart, 1.0f); }
        float   focusEnd() const                                                { return m_focusEnd; }
        void    setFocus(float start, float end);

protected:
        void    prepare() const;

private:
        Texture2D*      m_inputTexture;
        Texture2D*      m_depthTexture;
        Camera*         m_camera;
        Vec2            m_direction;

        int             m_maxNearRadius;
        int             m_maxFarRadius;
        float           m_focusStart;
        float           m_focusEnd;
};


/** @brief Постэффект для симулирования глубины резкости
 */
class DepthOfField : public PostEffect {
public:
        /// Конструктор
        DepthOfField();

        /// Обязательные параметры
        void            setInputTexture(Texture2D* texture)                     { m_inputTexture = texture; }
        void            setDepthTexture(Texture2D* texture)                     { m_depthblur.setDepthTexture(texture); }
        void            setActiveCamera(Camera* camera)                         { m_depthblur.setActiveCamera(camera); }

        /// Настройки прохода размытия
        DepthBlurPass*  depthBlurPass()                                         { return &m_depthblur; }

        /// Применяет постэффект
        Texture2D*      apply();

private:
        Texture2D*      m_inputTexture;
        RenderTarget    m_rt1;
        RenderTarget    m_rt2;

        DepthBlurPass   m_depthblur;

};


} //namespace imEngine

#endif // DEPTHOFFIELD_H
