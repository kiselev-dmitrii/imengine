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
class EmissiveMaterial : public EntityMaterial {
public:
        /// Конструктор
        EmissiveMaterial();

        /// Загружает параметры материала из JsonValue
        void            loadFromJson(const JsonValue& value);

        /// Устанавливает/возвращает цвет
        void            setEmissiveColor(const Vec3& color)                     { m_emissiveColor = color; }
        const Vec3&     emissiveColor() const                                   { return m_emissiveColor; }

        /// Включает/выключает материал
        void            bind();
        void            unbind();

private:
        Vec3    m_emissiveColor;
};


} //namespace imEngine

#endif // EMISSIVEMATERIAL_H
