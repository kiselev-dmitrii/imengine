#include "PointLight.h"
#include "../../Scene.h"

namespace imEngine {

PointLight::PointLight(Object *parent) :
        Light("lights/LightDeferred.glsl", parent),
        m_color(1.0, 1.0, 0.9)
{ }

void PointLight::bind(Scene* scene) {
        m_program->bind();

}

void PointLight::unbind() {
        m_program->unbind();
}

} //namespace imEngine
