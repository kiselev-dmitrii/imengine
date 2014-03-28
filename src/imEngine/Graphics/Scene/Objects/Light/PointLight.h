#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "Light.h"

namespace imEngine {

class PointLight : public Light {
public:
        /// Конструктор
        PointLight(Object* parent);

        /// Не поддерживает теневую карту
        void            calculateShadowMap()                                    { }

protected:
        void            prepare() const;
};

} //namespace imEngine

#endif // POINTLIGHT_H
