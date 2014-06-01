#ifndef ILIGHT_H
#define ILIGHT_H

#include "../Movable.h"
#include <imEngine/Graphics/GAPI/GAPI.h>
#include <imEngine/Graphics/Scene/PostEffects/Pass.h>
#include "ShadowTechnique.h"

namespace imEngine {


/** @brief Абстрактный класс освещения с отложенным рендером
 *
 * Перед использованием обязательно должен быть установлен GBuffer
 * Позволяет управлять общими характеристиками, такими как цвет света и его мощность
 */
class Light : public Movable, public Pass {
public:
        /// Конструктор. Регистрирует источник света на сцене
        Light(const String& filename, Object* parent);

        /// Загружает настройки из json-ноды
        void            loadFromJson(const JsonValue &node);
        /// Сохраняет настройки в виде json-ноды
        void            saveAsJson(JsonValue &result);

        /// Устанавливает G-buffer (обязательно)
        void            setDiffuseBuffer(Texture2D* texture)                    { m_diffuseBuffer = texture; }
        void            setMaterialBuffer(Texture2D* texture)                   { m_materialBuffer = texture; }
        void            setGeometryBuffer(Texture2D* texture)                   { m_geometryBuffer = texture; }
        void            setDepthBuffer(Texture2D* texture)                      { m_depthBuffer = texture; }

        /// Устанавливает/возвращает диффузный свет
        void            setDiffuseColor(const Vec3& color)                      { m_diffuseColor = glm::clamp(color, Vec3(0.0), Vec3(1.0)); }
        const Vec3&     diffuseColor() const                                    { return m_diffuseColor; }
        /// Устанавливает/возвращает отражательный свет
        void            setSpecularColor(const Vec3& color)                     { m_specularColor = glm::clamp(color, Vec3(0.0), Vec3(1.0)); }
        const Vec3&     specularColor() const                                   { return m_specularColor; }

        /// Устанавливает/возврашает коэффициент угасания (0 - нет угасания)
        void            setAttenuation(float attenuation)                       { m_attenuation = attenuation; }
        float           attenuation() const                                     { return m_attenuation; }
        /// Устанавливает/возвращает мощность источника света
        void            setPower(float power)                                   { m_power = power; }
        float           power() const                                           { return m_power; }

        /// Устанавливает метод генерирования теней. По-дефолту метод не используется (nullptr)
        void            setShadowTechnique(ShadowTechniquePtr technique);
        /// Расчитывает теневую карту для объектов на сцене, если задан метод генерирования
        void            calculateShadowMap();

protected:
        /// Устанавливает uniform-переменные, которые часто требуется передавать в шейдер для расчета освещения
        void            setGBuffers() const;
        void            setCameraSettings() const;
        void            setCommonLightSettings() const;

protected:
        Texture2D*      m_diffuseBuffer;
        Texture2D*      m_materialBuffer;
        Texture2D*      m_geometryBuffer;
        Texture2D*      m_depthBuffer;

        Vec3            m_diffuseColor;
        Vec3            m_specularColor;
        float           m_attenuation;
        float           m_power;

        ShadowTechniquePtr m_shadowTechnique;
};


} //namespace imEngine

#endif // ILIGHT_H
