#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "Light.h"

namespace imEngine {

class PointLight : public Light {
public:
        /// Конструктор
        PointLight(Object* parent);

        /// Устанавливает/возвращает цвет источника
        void            setColor(const Vec3& color)                             { m_color = color; }
        const Vec3&     color() const                                           { return m_color; }

        void            bind(Scene* scene);
        void            unbind();

private:
        Vec3            m_color;
};

} //namespace imEngine

#endif // POINTLIGHT_H
