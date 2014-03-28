#include "SpotLight.h"
#include "../../Scene.h"

namespace imEngine {

SpotLight::SpotLight(Object *parent) :
        Light("lights/SpotLightDeferred.glsl", parent)
{
        setCutoffAngle(20);
        setFalloffAngle(5);
}

void SpotLight::prepare() const {
        setGBuffers();
        setCameraSettings();
        setLightColors();

        // Установка позиции источника
        Vec3 positionVS = Vec3(scene()->activeCamera()->worldToLocalMatrix() * Vec4(worldPosition(), 1.0));
        m_program->setUniform("uLight.positionVS", positionVS);

        // Установка направления источника
        Vec3 negzPointWS = convertLocalToWorld(Vec3(0,0,-1));
        Vec3 negzPointVS = Vec3(scene()->activeCamera()->worldToLocalMatrix() * Vec4(negzPointWS, 1.0));
        Vec3 directionVS = glm::normalize(negzPointVS - positionVS);
        m_program->setUniform("uLight.directionVS", directionVS);

        // Установка углов развертки
        m_program->setUniform("uLight.cutoffAngle", m_cuttoffAngle);
        m_program->setUniform("uLight.falloffAngle", m_falloffAngle);
}

} //namespace imEngine
