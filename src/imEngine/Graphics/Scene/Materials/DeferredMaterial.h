#ifndef DEFERREDMATERIAL_H
#define DEFERREDMATERIAL_H

#include "Material.h"

namespace imEngine {

/** @brief Класс представляет собой текстурированый материал для отложенного шейдера
 *
 * Материал основан на содержимом трех текстур:
 *      - DiffuseTexture - содержит основной цвет
 *      - NormalTexture - содержит нормаль в tangentCoords
 *      - SpecularTexture - текстура в оттенках серого, содержит параметры отражения
 */
class TexturedDeferredMaterial : public Material {
public:
        /// Конструктор
        TexturedDeferredMaterial();

        /// Загружает параметры текстуры с XML ноды
        void            loadFromXML(const XmlNode& node);

        /// Устанавливает масштаб
        void            setScale(const Vec2& scale);

        /// Устанавливает диффузную/нормальную/спекулярную текстуру
        void            setDiffuseTexture(const String& name);
        void            setSpecularTexture(const String& name);
        void            setNormalTexture(const String& name);

        /// Устанавливает/отключает материал
        void            bind();
        void            unbind();

private:
        Vec2            m_scale;
        Texture2D*      m_diffuseTexture;
        Texture2D*      m_specularTexture;
        Texture2D*      m_normalTexture;
};

} //namespace imEngine

#endif // DEFERREDMATERIAL_H
