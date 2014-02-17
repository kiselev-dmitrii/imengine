#ifndef BASICDEFERREDMATERIAL_H
#define BASICDEFERREDMATERIAL_H

#include "Material.h"
#include <imEngine/FileContainers/Xml.h>

namespace imEngine {

class BasicDeferredMaterial : public Material {
public:
        /// Конструктор
        BasicDeferredMaterial();
        /// Загружает параметры с XML ноды
        void            loadFromXML(const XmlNode& node);

        void            setDiffuseColor(const Vec3& color)                      { m_diffuseColor = color; }
        const Vec3&     diffuseColor() const                                    { return m_diffuseColor; }

        void            setSpecularLevel(float level)                           { m_specularLevel = level; }
        float           specularLevel() const                                   { return m_specularLevel; }

        void            setSpecularExponent(float exponent)                     { m_specularExponent = exponent; }
        float           specularExponent() const                                { return m_specularExponent; }

        void            setMetallicLevel(float level)                           { m_metallicLevel = level; }
        float           metallicLevel() const                                   { return m_metallicLevel; }

        void            bind();
        void            unbind();

private:
        Vec3    m_diffuseColor;
        float   m_specularLevel;
        float   m_specularExponent;
        float   m_metallicLevel;

};

} //namespace imEngine

#endif // BASICDEFERREDMATERIAL_H
