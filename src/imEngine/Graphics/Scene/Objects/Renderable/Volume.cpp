#include "Volume.h"
#include "../ObjectManager.h"
#include <imEngine/Utils/Debug.h>

namespace imEngine {


Volume::Volume(Texture3D *texture, Object *parent) :
        Movable(parent)
{
        OBJECTS->registerVolume(this);
        m_program = RESOURCES->programs()->program("VolumeRenderer.glsl");
        setTexture(texture);
        m_cube = Geometry::box(Vec3(-0.5,-0.5,-0.5), Vec3(0.5,0.5,0.5));
}

void Volume::setTexture(Texture3D *texture) {
        m_texture = texture;

        m_texture->setWrap(TextureWrapMode::CLAMP_TO_BORDER);
        m_texture->setMagnificationFilter(TextureMagFilter::LINEAR);
        m_texture->setMinimizationFilter(TextureMinFilter::LINEAR_MIPMAP_LINEAR);
        m_texture->setMinMipLevel(0);
        m_texture->setMaxMipLevel(4);
        m_texture->generateMipmaps();
}

Texture3D* Volume::texture() {
        return m_texture;
}

void Volume::render() {
        const Mat4& viewMatrix = OBJECTS->currentCamera()->worldToLocalMatrix();
        const Mat4& projectionMatrix = OBJECTS->currentCamera()->viewToClipMatrix();
        const Mat4& modelMatrix = localToWorldMatrix();

        Mat4 modelViewProjectionMatrix = projectionMatrix * viewMatrix * modelMatrix;

        Vec3 worldSpaceCameraPosition = OBJECTS->currentCamera()->worldPosition();
        Vec3 objectSpaceCameraPosition = Vec3(worldToLocalMatrix() * Vec4(worldSpaceCameraPosition, 1.0));

        m_program->bind();
        m_program->setUniform("uModelViewProjectionMatrix", modelViewProjectionMatrix);
        m_texture->bind(0);
        m_program->setUniform("uVolumeTexture", 0);
        m_program->setUniform("uObjectSpaceCameraPosition", objectSpaceCameraPosition);
        m_cube->render();
}


} //namespace imEngine

