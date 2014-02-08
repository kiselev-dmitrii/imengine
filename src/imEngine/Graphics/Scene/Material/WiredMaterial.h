#ifndef WIREDMATERIAL_H
#define WIREDMATERIAL_H

#include "Material.h"

namespace imEngine {

/** @brief Сетка.
 */
class WiredMaterial : public MaterialImplementation<WiredMaterial> {
public:
        /// Конструктор
        WiredMaterial();

        /// Устанавливает/возвращает размер границы сетки
        void            setBorderColor(const Vec4& color)                       { m_borderColor = color; }
        const Vec4&     borderColor() const                                     { return m_borderColor; }

        /// Устанавливает/возвращает основной цвет
        void            setBaseColor(const Vec4& color)                         { m_baseColor = color; }
        const Vec4&     baseColor() const                                       { return m_baseColor; }

        /// Устанавливает/возвращает ширину границы
        void            setBorderWidth(float width)                             { m_borderWidth = width; }
        float           borderWidth()                                           { return m_borderWidth; }

        /// Устанавливает/возвращает масштаб сетки
        void            setScale(float scale)                                   { m_scale = scale; }
        float           scale() const                                           { return m_scale; }

        ///Перегруженные методы
        void            bind();
        void            unbind();

private:
        Vec4                    m_borderColor;
        Vec4                    m_baseColor;
        float                   m_borderWidth;
        float                   m_scale;
};

} //namespace imEngine

#endif // WIREDMATERIAL_H
