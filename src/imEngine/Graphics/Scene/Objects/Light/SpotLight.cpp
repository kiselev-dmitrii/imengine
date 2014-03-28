#include "SpotLight.h"
#include "../../Scene.h"
#include "../../ResourceManager.h"

namespace imEngine {

SpotLight::SpotLight(Object *parent) :
        Light("lights/SpotLightDeferred.glsl", parent),
        m_rt(IVec2(64,64))
{
        setCutoffAngle(20);
        setFalloffAngle(5);

        ///Возможной уйдет в другой класс
        m_shadowProgram = RESOURCES->programs()->program("lights/VSM.glsl");
        m_frustum.setAspectRatio(1.0);
        m_rt.enableColorBuffer(0, InternalFormat::COLOR_FLOAT_3_COMP_32_BIT, true);
        m_rt.enableDepthBuffer(InternalFormat::DEPTH_NORM_1_COMP_24_BIT, false);
}

void SpotLight::calculateShadowMap() {
        m_rt.bind();
                Renderer::clearBuffers();
                Renderer::setDepthMode(DepthMode::LESS);
                const Mat4& projectionMatrix = m_frustum.viewToClipMatrix();
                const Mat4& viewMatrix = worldToLocalMatrix();

                for(Polygonal* object: scene()->polygonals()) {
                        const Mat4& modelMatrix = object->localToWorldMatrix();
                        Mat4 modelViewProjectionMatrix = projectionMatrix * viewMatrix * modelMatrix;

                        for (ModelDetail& detail: object->model().details()) {
                                if (detail.material->type() == MaterialType::DEFERRED) {
                                        m_shadowProgram->bind();
                                        m_shadowProgram->setUniform("uModelViewProjectionMatrix", modelViewProjectionMatrix);
                                        detail.geometry->render();
                                        m_shadowProgram->unbind();
                                }
                        }
                }
                Renderer::setDepthMode(DepthMode::NONE);
        m_rt.unbind();
}

void SpotLight::setCutoffAngle(float angle) {
        m_cuttoffAngle = glm::clamp(angle, 0.0f, 90.0f);
        m_frustum.setFieldOfView(angle);
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

        // Установка теневой карты
        m_rt.colorBufferTexture(0)->bind(4);
        m_program->setUniform("uLight.shadowMap", 4);

        // Установка матрицы, переводящей вершины:
        // ViewSpace -> WorldSpace -> LightSpace -> LightClipSpace
        Mat4 shadowMatrix = m_frustum.viewToClipMatrix() *
                            this->worldToLocalMatrix() *
                            scene()->activeCamera()->localToWorldMatrix();
        m_program->setUniform("uLight.shadowMatrix", shadowMatrix);
}

} //namespace imEngine
