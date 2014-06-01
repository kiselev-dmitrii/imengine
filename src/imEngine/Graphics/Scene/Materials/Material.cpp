#include "Material.h"
#include "../ResourceManager.h"

namespace imEngine {

//######################### Material ###################################//

Material::Material(const String &program, MaterialType type) {
        m_program = RESOURCES->programs()->program(program);
        m_type = type;
}

Program* Material::program() const {
        return m_program;
}

MaterialType Material::type() const {
        return m_type;
}

//######################### VolumeMaterial ###################################//

VolumeMaterial::VolumeMaterial(const String &filename) :
        Material(filename, MaterialType::UNLIGHTENED),
        m_texture(nullptr),
        m_camera(nullptr),
        m_object(nullptr)
{
        setStep(1);
}

void VolumeMaterial::loadFromJson(const JsonValue &node) {
        JsonValue step = node["step"];
        JsonValue densityTexture = node["density_texture"];

        if (!step.isNull()) setStep(step.asInt());
        if (!densityTexture.isNull()) setDensityTexture(densityTexture.asString());
}

void VolumeMaterial::setData(const String &name, const IVec3& size, InternalFormat::Enum internal) {
        setData(RESOURCES->textures()->texture3D(name, size, internal));
}

void VolumeMaterial::setData(Texture3D *data) {
        m_texture = data;

        m_texture->setWrap(TextureWrapMode::CLAMP_TO_BORDER);
        m_texture->setMagnificationFilter(TextureMagFilter::LINEAR);
        m_texture->setMinimizationFilter(TextureMinFilter::LINEAR_MIPMAP_LINEAR);
        m_texture->setMinMipLevel(0);
        m_texture->setMaxMipLevel(4);
        m_texture->generateMipmaps();
}

Texture3D* VolumeMaterial::data() const {
        return m_texture;
}

void VolumeMaterial::setDensityTexture(const String &path) {
        m_densityTexture.reset(new Texture2D());
        m_densityTexture->load(path);
}

void VolumeMaterial::bind() {
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
        m_densityTexture->bind(1);
        m_program->setUniform("uDensityTexture", 1);
        m_program->setUniform("uObjectSpaceCameraPosition", objectSpaceCameraPosition);
        m_program->setUniform("uStep", m_step);
}

void VolumeMaterial::unbind() {
        m_program->unbind();
}

} //namespace imEngine
