#include "PointLight.h"
#include "../../Scene.h"

namespace imEngine {

PointLight::PointLight(Object *parent) :
        Light("lights/LightDeferred.glsl", parent),
        m_color(1.0, 1.0, 0.9)
{ }

void PointLight::bind(Scene* scene) {
        m_program->bind();

        Camera* cam = scene->activeCamera();
        float farDistance = cam->farDistance();
        float aspectRatio = cam->aspectRatio();
        float tanFovyDiv2 = glm::tan(glm::radians(cam->fieldOfView() / 2));

        m_program->setUniform("uFarDistance", farDistance);
        m_program->setUniform("uAspectRatio", aspectRatio);
        m_program->setUniform("uTanFovyDiv2", tanFovyDiv2);

        Vec3 diffuseColor = m_color;
        Vec3 specularColor = Vec3(1.0);
        Vec3 positionVS = Vec3(cam->worldToLocalMatrix() * Vec4(worldPosition(), 1.0));

        m_program->setUniform("uLight.diffuse", diffuseColor);
        m_program->setUniform("uLight.specular", specularColor);
        m_program->setUniform("uLight.positionVS", positionVS);

}

void PointLight::unbind() {
        m_program->unbind();
}

} //namespace imEngine
