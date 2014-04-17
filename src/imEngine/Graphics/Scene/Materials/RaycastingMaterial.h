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

        /// Устанавливает минимальную плотность
        void            setMinDensity(float density)                            { m_minDensity = glm::clamp(density, 0.0f, 1.0f); }
        float           minDensity() const                                      { return m_minDensity; }

        /// Устанавливает максимальную плотность
        void            setMaxDensity(float density)                            { m_maxDensity = glm::clamp(density, 0.0f, 1.0f); }
        float           maxDensity() const                                      { return m_maxDensity; }

        /// Устанавливает плоскость отсечения
        void            setClipPlane(const Vec4& plane);
        Vec4            clipPlane() const;

        /// Перегруженный метод установки материала
        void            bind();

private:
        float           m_minDensity;
        float           m_maxDensity;
        Vec4            m_clipPlane;
};


} //namespace imEngine


#endif // RAYCASTINGMATERIAL_H
