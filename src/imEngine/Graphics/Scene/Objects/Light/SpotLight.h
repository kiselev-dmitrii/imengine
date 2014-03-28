#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

#include "Light.h"

namespace imEngine {

/** @brief Прожекторный консный тип источника света
 */
class SpotLight : public Light {
public:
        /// Конструктор
        SpotLight(Object* parent);

        /// Устанавливает телесный угол развертки конуса
        void    setCutoffAngle(float angle)                                     { m_cuttoffAngle = glm::clamp(angle, 0.0f, 90.0f); }
        float   cutoffAngle() const                                             { return m_cuttoffAngle; }
        /// Устанавливает внутренний угол затухания
        void    setFalloffAngle(float angle)                                    { m_falloffAngle = glm::clamp(angle, 0.0f, m_cuttoffAngle); }
        float   falloffAngle() const                                            { return m_falloffAngle; }

protected:
        void    prepare() const;

private:
        float   m_cuttoffAngle;
        float   m_falloffAngle;

};

} //namespace imEngine

#endif // SPOTLIGHT_H
