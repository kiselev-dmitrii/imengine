#include "PointLight.h"
#include "../../Scene.h"

namespace imEngine {

PointLight::PointLight(Object *parent) :
        Light("lights/PointLightDeferred.glsl", parent)
{ }

void PointLight::prepare() const {
        setGBuffers();
        setCameraSettings();
        setCommonLightSettings();

        /// Установка позиции
        Vec3 positionVS = Vec3(scene()->activeCamera()->worldToLocalMatrix() * Vec4(worldPosition(), 1.0));
        m_program->setUniform("uLight.positionVS", positionVS);
}

} //namespace imEngine
