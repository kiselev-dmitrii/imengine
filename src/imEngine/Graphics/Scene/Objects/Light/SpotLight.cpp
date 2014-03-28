#include "SpotLight.h"
#include "../../Scene.h"

namespace imEngine {

SpotLight::SpotLight(Object *parent) :
        Light("lights/SpotLightDeferred.glsl", parent)
{
        m_cuttoffAngle = 20;
        m_falloffAngle = 5;
}

void SpotLight::bind(Scene *scene) {
        m_program->bind();
        Camera* cam = scene->activeCamera();

        /// Камера
        m_program->setUniform("uNearDistance", cam->nearDistance());
        m_program->setUniform("uFarDistance", cam->farDistance());
        m_program->setUniform("uInvProjectionMatrix", glm::inverse(cam->viewToClipMatrix()));

        /// Настройки освещения
        Vec3 diffuse = Vec3(1.3);
        Vec3 specular = Vec3(1.0);
        Vec3 positionVS = Vec3(cam->worldToLocalMatrix() * Vec4(worldPosition(), 1.0));
        /// Вычисление направление камеры
        Vec3 negzPointWS = convertLocalToWorld(Vec3(0,0,-1));
        Vec3 negzPointVS = Vec3(cam->worldToLocalMatrix() * Vec4(negzPointWS, 1.0));
        Vec3 directionVS = glm::normalize(negzPointVS - positionVS);

        m_program->setUniform("uLight.diffuse", diffuse);
        m_program->setUniform("uLight.specular", specular);
        m_program->setUniform("uLight.cutoffAngle", m_cuttoffAngle);
        m_program->setUniform("uLight.falloffAngle", m_falloffAngle);
        m_program->setUniform("uLight.directionVS", directionVS);
        m_program->setUniform("uLight.positionVS", positionVS);

}

void SpotLight::unbind() {
        m_program->unbind();
}

} //namespace imEngine
