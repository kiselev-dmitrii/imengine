#include "Skybox.h"
#include "ResourceManager.h"

namespace imEngine {

Skybox::Skybox(const String &filename) {
        CubeTexturePtr texture(new CubeTexture());
        texture->load(filename);
        setTexture(texture);
        m_program = RESOURCES->programs()->program("SkyboxRender.glsl");
        m_cube = Geometry::cube();
}

Skybox::Skybox(const CubeTexturePtr texture) {
        setTexture(texture);
        m_program = RESOURCES->programs()->program("SkyboxRender.glsl");
        m_cube = Geometry::cube();
}

void Skybox::setTexture(CubeTexturePtr texture) {
        texture->setMagnificationFilter(TextureMagFilter::LINEAR);
        texture->setMinimizationFilter(TextureMinFilter::LINEAR);
        texture->setWrap(TextureWrapMode::CLAMP_TO_EDGE);
        m_texture = texture;
}

CubeTexturePtr Skybox::texture() const {
        return m_texture;
}

void Skybox::render(const Mat3 &viewRotationMatrix, const Mat4 &projectionMatrix) {
        m_program->bind();
        m_program->setUniform("uViewRotationMatrix", viewRotationMatrix);
        m_program->setUniform("uProjectionMatrix", projectionMatrix);
        m_texture->bind(0);
        m_program->setUniform("uEnvironment", 0);
        m_cube->render();
}

} //namespace imEngine
