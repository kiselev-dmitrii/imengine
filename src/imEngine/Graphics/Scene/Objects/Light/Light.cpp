#include "Light.h"
#include "../../Scene.h"

namespace imEngine {

Light::Light(Object* parent) :
        Movable(parent)
{
        scene()->registerLight(this);
}

} //namespace imEngine
