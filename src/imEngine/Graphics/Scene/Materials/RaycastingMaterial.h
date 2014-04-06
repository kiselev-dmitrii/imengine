#ifndef RAYCASTINGMATERIAL_H
#define RAYCASTINGMATERIAL_H

#include "Material.h"

namespace imEngine {


/** @brief Основной материал для отображения Volume моделей
 */
class RaycastingMaterial : public VolumeMaterial {
public:
        /// Конструктор
        RaycastingMaterial();

        /// Устанавливает шаг трассировки в диапазоне [0;1]
        void    setStep(float step)                                             { m_step = glm::clamp(step, 0.0f, 1.0f); }
        float   step() const                                                    { return m_step; }

        /// Устанавливает минимальную плотность
        void    setMinDensity(float density)                                    { m_minDensity = glm::clamp(density, 0.0f, 1.0f); }
        float   minDensity() const                                              { return m_minDensity; }
        /// Устанавливает максимальную плотность
        void    setMaxDensity(float density)                                    { m_maxDensity = glm::clamp(density, 0.0f, 1.0f); }
        float   maxDensity() const                                              { return m_maxDensity; }

        /// Включает/выключает материал
        void    bind();
        void    unbind();

private:
        float   m_step;
        float   m_minDensity;
        float   m_maxDensity;
};


} //namespace imEngine


#endif // RAYCASTINGMATERIAL_H
