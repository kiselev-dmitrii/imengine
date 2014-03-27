#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

#include "Light.h"

namespace imEngine {

/** @brief Прожекторный консный тип источника света
 *
 * Поддерживает тени
 */
class SpotLight : public Light {
public:
        /// Конструктор
        SpotLight(Object* parent);

        /// Устанавливает телесный угол развертки конуса (в градусах)(между высотой и боковой поверхностью)
        void    setCutoffAngle(float angle)                                     { m_cuttoffAngle = angle; }
        /// Устанавливает угол области затухания
        void    setFalloffAngle(float angle)                                    { m_falloffAngle = angle; }

        /// Устанавливает/сбрасывает настройки
        void    bind(Scene* scene);
        void    unbind();

private:
        float   m_cuttoffAngle;
        float   m_falloffAngle;

};

} //namespace imEngine

#endif // SPOTLIGHT_H
