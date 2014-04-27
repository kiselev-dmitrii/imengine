#ifndef GENERICMATERIAL_H
#define GENERICMATERIAL_H

#include "Material.h"

namespace imEngine {

/** @brief Класс представляет собой основной материал для большинства объектов на сцене
 *
 * Материал
 */
class GenericMaterial : public EntityMaterial {
public:
        /// Конструктор
        GenericMaterial();

        /// Загружает параметры материала из Json элемента
        void            loadFromJson(const JsonValue& value);

        /// Устанавливает/возвращает окружающий цвет
        void            setAmbientColor(const Vec3& color)                      { m_ambientColor = color; }
        const Vec3&     ambientColor() const                                    { return m_ambientColor; }
        /// Устанавливает/возвращает диффузный цвет
        void            setDiffuseColor(const Vec3& color)                      { m_diffuseColor = color; }
        const Vec3&     diffuseColor() const                                    { return m_diffuseColor; }
        /// Устанавливает/возвращает отражающий цвет
        void            setSpecularColor(const Vec3& color)                     { m_specularColor = color; }
        const Vec3&     specularColor() const                                   { return m_specularColor; }
        /// Устанавливает степень отражения
        void            setSpecularPower(float power)                           { m_specularPower = power; }
        float           specularPower() const                                   { return m_specularPower; }

        /// Устанавливает/возвращает диффузную текстру
        void            setDiffuseTexture(const String& path);
        Texture2D*      diffuseTexture() const                                  { return m_diffuseTexture; }
        /// Устанавливает/возвращает отражающую текстуру
        void            setSpecularTexture(const String& path);
        Texture2D*      specularTexture() const                                 { return m_specularTexture; }
        /// Устанавливает/возвращает нормальную текстуру
        void            setNormalTexture(const String& path);
        Texture2D*      normalTexture() const                                   { return m_normalTexture; }

        /// Устанавливает/отключает материал
        void            bind();
        void            unbind();

private:
        Vec3            m_ambientColor;
        Vec3            m_diffuseColor;
        Vec3            m_specularColor;
        float           m_specularPower;

        Texture2D*      m_diffuseTexture;
        Texture2D*      m_specularTexture;
        Texture2D*      m_normalTexture;
};

} //namespace imEngine

#endif // GENERICMATERIAL_H
