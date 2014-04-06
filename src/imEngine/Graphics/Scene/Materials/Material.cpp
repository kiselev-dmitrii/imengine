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
        m_camera(nullptr)
{ }

void VolumeMaterial::setData(const String &name, const IVec3& size, InternalFormat::Enum internal) {
        setData(RESOURCES->textures()->texture3D(name, size, internal));
}

void VolumeMaterial::setData(Texture3D *data) {
        m_texture = data;

        m_texture->setWrap(TextureWrapMode::CLAMP_TO_EDGE);
        m_texture->setMagnificationFilter(TextureMagFilter::LINEAR);
        m_texture->setMinimizationFilter(TextureMinFilter::LINEAR_MIPMAP_LINEAR);
        m_texture->setMinMipLevel(0);
        m_texture->setMaxMipLevel(4);
        m_texture->generateMipmaps();
}

Texture3D* VolumeMaterial::data() const {
        return m_texture;
}

void VolumeMaterial::setActiveCamera(Camera *camera) {
        m_camera = camera;
}

void VolumeMaterial::setObject(Object *object) {
        m_object = object;
}

} //namespace imEngine
