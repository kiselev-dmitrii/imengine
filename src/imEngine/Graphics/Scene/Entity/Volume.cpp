#include "Volume.h"
#include "../Scene.h"
#include <imEngine/Utils/Debug.h>

namespace imEngine {


ProgramPtr Volume::s_program;

Volume::Volume(Texture3D *texture, SceneObject *parent) :
        Entity(parent)
{
        initProgram();
        setTexture(texture);

        // Создаем куб. Такие координаты удобны тем, что из них можно получить
        // _трехмерные_ текстурные координаты просто прибавляя 0.5 к координатам вершин
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
        const Mat4& viewMatrix = scene()->currentCamera()->worldToLocalMatrix();
        const Mat4& projectionMatrix = scene()->currentCamera()->viewToClipMatrix();
        const Mat4& modelMatrix = localToWorldMatrix();

        Mat4 modelViewProjectionMatrix = projectionMatrix * viewMatrix * modelMatrix;

        Vec3 worldSpaceCameraPosition = scene()->currentCamera()->worldPosition();
        Vec3 objectSpaceCameraPosition = Vec3(worldToLocalMatrix() * Vec4(worldSpaceCameraPosition, 1.0));

        s_program->bind();
        s_program->setUniform("uModelViewProjectionMatrix", modelViewProjectionMatrix);
        m_texture->bind(0);
        s_program->setUniform("uVolumeTexture", 0);
        s_program->setUniform("uObjectSpaceCameraPosition", objectSpaceCameraPosition);
        m_cube->render();
}

void Volume::initProgram() {
        static bool wasInited = false;
        if (wasInited) return;

        s_program = ProgramPtr(new Program());
        s_program->loadSourceFromFile("resources/shaders/VolumeRender.glsl");
        s_program->build();

        wasInited = true;
}


} //namespace imEngine

