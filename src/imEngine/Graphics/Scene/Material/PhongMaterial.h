#ifndef PHONGMATERIAL_H
#define PHONGMATERIAL_H

#include "Material.h"

namespace imEngine {

/** Материал по Фонгу. Подходит для имитации пластика
 */
class PhongMaterial : MaterialImplementation<PhongMaterial> {
public:
        /// Конструктор
        PhongMaterial();

        void            setAmbientColor(const Vec3& color)                      { m_ambientColor = color; }
        const Vec3&     ambientColor() const                                    { return m_ambientColor; }

        void            setDiffuseColor(const Vec3& color)                      { m_diffuseColor = color; }
        const Vec3&     diffuseColor() const                                    { return m_diffuseColor; }

        void            setSpecularColor(const Vec3& color)                     { m_specularColor = color; }
        const Vec3&     specularColor() const                                   { return m_specularColor; }

        void            setShininess(float shininess)                           { m_shininess = shininess; }
        float           shininess() const                                       { return m_shininess; }

        void            bind();
        void            unbind();

private:
        Vec3    m_ambientColor;
        Vec3    m_diffuseColor;
        Vec3    m_specularColor;
        float   m_shininess;
};

} //namespace imEngine

#endif // PHONGMATERIAL_H
