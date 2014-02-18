#include "PointLight.h"
#include "../../Scene.h"

namespace imEngine {

PointLight::PointLight(Object *parent) :
        Light("lights/LightDeferred.glsl", parent),
        m_color(1.0, 1.0, 0.9)
{ }

void PointLight::bind(Scene* scene) {
        float farDistance = scene->activeCamera()->farDistance();
        float tanHalfFovy = glm::tan(scene->activeCamera()->fieldOfView() / 2.0);
        float aspectRatio = scene->activeCamera()->aspectRatio();

        Vec3 viewSpaceLightPosition = Vec3(scene->activeCamera()->worldToLocalMatrix() * Vec4(worldPosition(), 1.0));

        m_program->bind();
        m_program->setUniform("uFarDistance", farDistance);
        m_program->setUniform("uTanHalfFovy", tanHalfFovy);
        m_program->setUniform("uAspectRatio", aspectRatio);

        m_program->setUniform("uDiffuseColor", m_color);
        m_program->setUniform("uViewSpaceLightPosition", viewSpaceLightPosition);
}

void PointLight::unbind() {
        m_program->unbind();
}

} //namespace imEngine
