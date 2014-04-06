#ifndef ISOSURFACEMATERIAL_H
#define ISOSURFACEMATERIAL_H

#include "Material.h"

namespace imEngine {


/** @brief Основной материал для отображения Volume моделей
 */
class IsosurfaceMaterial : public VolumeMaterial {
public:
        /// Конструктор
        IsosurfaceMaterial();

        /// Устанавливает шаг трассировки в диапазоне [0;1]
        void    setStep(float step)                                             { m_step = glm::clamp(step, 0.0f, 1.0f); }
        float   step() const                                                    { return m_step; }

        /// Устанавливает пороговую плотность
        void    setThresholdDensity(float density)                              { m_thresholdDensity = density; }
        float   thresholdDensity() const                                        { return m_thresholdDensity; }

        /// Включает/выключает материал
        void    bind();
        void    unbind();

private:
        float   m_step;
        float   m_thresholdDensity;
};


} //namespace imEngine

#endif // ISOSURFACEMATERIAL_H
