#include "IsosurfaceMaterial.h"

namespace imEngine {

IsosurfaceMaterial::IsosurfaceMaterial() :
        VolumeMaterial("materials/IsosurfaceMaterial.glsl")
{
        setStep(0.01);
        setThresholdDensity(0.2);
}

void IsosurfaceMaterial::bind() {
        const Mat4& viewMatrix = m_camera->worldToLocalMatrix();
        const Mat4& projectionMatrix = m_camera->viewToClipMatrix();
        const Mat4& modelMatrix = m_object->localToWorldMatrix();

        Mat4 modelViewProjectionMatrix = projectionMatrix * viewMatrix * modelMatrix;
        Vec3 worldSpaceCameraPosition = m_camera->worldPosition();
        Vec3 objectSpaceCameraPosition = Vec3(m_object->worldToLocalMatrix() * Vec4(worldSpaceCameraPosition, 1.0));

        m_program->bind();
        m_program->setUniform("uModelViewProjectionMatrix", modelViewProjectionMatrix);
        m_texture->bind(0);
        m_program->setUniform("uVolumeTexture", 0);
        m_program->setUniform("uObjectSpaceCameraPosition", objectSpaceCameraPosition);
        m_program->setUniform("uStep", m_step);
        m_program->setUniform("uThresholdDensity", m_thresholdDensity);
}

void IsosurfaceMaterial::unbind() {
        m_program->unbind();
}

} //namespace imEngine
