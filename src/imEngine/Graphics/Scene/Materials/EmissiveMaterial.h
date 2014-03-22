#ifndef EMISSIVEMATERIAL_H
#define EMISSIVEMATERIAL_H

#include "Material.h"

namespace imEngine {

/** @brief Материал, который излучает цвет
 *
 * Грубо говоря, на него не действуют источники света,
 * При рендере, детали с этим материалом записываются сразу в LightBuffer, исключая
 * запись в GBuffer.
 */
class EmissiveMaterial : public Material {
public:
        /// Конструктор
        EmissiveMaterial();

        void    setColor(const Vec3& color);
        void    setPower(float power);

        void    bind();
        void    unbind();

private:
        Vec3    m_color;
        float   m_power;
};

} //namespace imEngine

#endif // EMISSIVEMATERIAL_H
