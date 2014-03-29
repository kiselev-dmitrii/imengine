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
};

} //namespace imEngine

#endif // SPOTLIGHT_H
