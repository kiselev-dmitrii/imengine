#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

#include "Light.h"
#include "../Camera/Frustum.h"
#include <imEngine/Graphics/RenderTarget.h>

namespace imEngine {

/** @brief Прожекторный консный тип источника света
 */
class SpotLight : public Light {
public:
        /// Конструктор
        SpotLight(Object* parent);

        /// Рассчитывает теневую карту
        void    calculateShadowMap();

        /// Устанавливает телесный угол развертки конуса
        void    setCutoffAngle(float angle);
        float   cutoffAngle() const;
        /// Устанавливает внутренний угол затухания
        void    setFalloffAngle(float angle);
        float   falloffAngle() const;

protected:
        void    prepare() const;

private:
        float           m_cuttoffAngle;
        float           m_falloffAngle;

        /// Для генерирования теней
        Program*        m_shadowProgram;
        Frustum         m_frustum;
        RenderTarget    m_rt;
};

} //namespace imEngine

#endif // SPOTLIGHT_H
