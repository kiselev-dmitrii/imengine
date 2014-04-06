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

        /// Включает/выключает материал
        void    bind();
        void    unbind();

private:
        float   m_step;
};


} //namespace imEngine


#endif // RAYCASTINGMATERIAL_H
