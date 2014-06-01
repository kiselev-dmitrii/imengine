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

        /// Загружает настройки из json-ноды
        void    loadFromJson(const JsonValue &node);

        /// Устанавливает пороговую плотность
        void    setThresholdDensity(float density)                              { m_thresholdDensity = glm::clamp(density, 0.0f, 1.0f); }
        float   thresholdDensity() const                                        { return m_thresholdDensity; }

        /// Перегруженный метод установки материала
        void    bind();

private:
        float   m_thresholdDensity;
};


} //namespace imEngine

#endif // ISOSURFACEMATERIAL_H
