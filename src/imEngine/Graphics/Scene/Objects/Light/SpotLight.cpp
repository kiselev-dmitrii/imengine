#include "SpotLight.h"
#include "../../Scene.h"
#include "../../ResourceManager.h"
#include <glm/gtc/matrix_transform.hpp>

namespace imEngine {

SpotLight::SpotLight(Object *parent) :
        Light("lights/SpotLightDeferred.glsl", parent)
{
        setCutoffAngle(20);
        setFalloffAngle(5);
        setShadowTechnique(ShadowTechniquePtr(new SimpleShadowMapping()));
}

void SpotLight::setCutoffAngle(float angle) {
        m_cuttoffAngle = glm::clamp(angle, 0.0f, 90.0f);
        if (m_shadowTechnique) m_shadowTechnique->frustum()->setFieldOfView(2*angle);
}

float SpotLight::cutoffAngle() const {
        return m_cuttoffAngle;
}

void SpotLight::setFalloffAngle(float angle) {
        m_falloffAngle = glm::clamp(angle, 0.0f, m_cuttoffAngle);
}

float SpotLight::falloffAngle() const {
        return m_falloffAngle;
}

void SpotLight::prepare() const {
        setGBuffers();
        setCameraSettings();
        setCommonLightSettings();

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

        // Установка теневой карты
        if (m_shadowTechnique) {
                m_shadowTechnique->shadowMap()->bind(4);
                m_program->setUniform("uShadow.map", 4);

                // Установка матрицы, переводящей вершины:
                // ViewSpace -> WorldSpace -> LightSpace -> LightClipSpace
                Mat4 bias = glm::translate(Mat4(1), Vec3(0.5))*glm::scale(Mat4(1), Vec3(0.5));
                Mat4 shadowMatrix = bias *
                            m_shadowTechnique->frustum()->viewToClipMatrix() *
                            this->worldToLocalMatrix() *
                                    scene()->activeCamera()->localToWorldMatrix();
                m_program->setUniform("uShadow.VStoSTS", shadowMatrix);
        }
}

} //namespace imEngine
