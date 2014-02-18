#include "Light.h"
#include "../../Scene.h"
#include "../../ResourceManager.h"

namespace imEngine {

Light::Light(const String& filename, Object* parent) :
        Movable(parent)
{
        scene()->registerLight(this);
        m_program = RESOURCES->programs()->program(filename);
}

Program* Light::program() const {
        return m_program;
}

} //namespace imEngine
