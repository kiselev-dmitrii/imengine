#include "ShadowTechnique.h"
#include "../../ResourceManager.h"
#include "../../Scene.h"

namespace imEngine {

ShadowTechnique::ShadowTechnique(const String &filename, const StringList &defines) :
        m_size(256, 256),
        m_defines(defines),
        m_shadowMap(nullptr)
{
        m_program = RESOURCES->programs()->program(filename);
        m_frustum.setAspectRatio(1.0);
}

void ShadowTechnique::setShadowMapSize(const IVec2 &size) {
        m_size = size;
        m_frustum.setAspectRatio(((float)m_size.x)/m_size.y);
        updateSize(size);
}

const IVec2& ShadowTechnique::shadowMapSize() const {
        return m_size;
}

Frustum* ShadowTechnique::frustum() {
        return &m_frustum;
}

const StringList& ShadowTechnique::defines() const {
        return m_defines;
}

Texture2D* ShadowTechnique::shadowMap() {
        return m_shadowMap;
}

void ShadowTechnique::renderObjects(Light *light, PolygonalList &objects) {
        const Mat4& projectionMatrix = m_frustum.viewToClipMatrix();
        const Mat4& viewMatrix = light->worldToLocalMatrix();

        for(Polygonal* object: objects) {
                const Mat4& modelMatrix = object->localToWorldMatrix();
                Mat4 modelViewProjectionMatrix = projectionMatrix * viewMatrix * modelMatrix;

                for (ModelDetail& detail: object->model().details()) {
                        if (detail.material->type() == MaterialType::DEFERRED) {
                                m_program->bind();
                                m_program->setUniform("uModelViewProjectionMatrix", modelViewProjectionMatrix);
                                detail.geometry->render();
                                m_program->unbind();
                        }
                }

        }
}


//########################## SimpleShadowMapping #############################//

SimpleShadowMapping::SimpleShadowMapping() :
        ShadowTechnique("lights/WriteDepth.glsl", {"SHADOW_MAPPING"}),
        m_rt(m_size)
{
        setShadowMapSize(IVec2(1000, 1000));

        m_rt.enableDepthBuffer(InternalFormat::DEPTH_NORM_1_COMP_24_BIT, true);

        m_rt.depthBufferTexture()->setMinimizationFilter(TextureMinFilter::LINEAR);
        m_rt.depthBufferTexture()->setMagnificationFilter(TextureMagFilter::LINEAR);
        m_rt.depthBufferTexture()->setCompareMode(TextureCompareMode::REF_TO_TEXTURE);
        m_rt.depthBufferTexture()->setCompareFunction(TextureCompareFunction::LESS);
        m_rt.depthBufferTexture()->setWrap(TextureWrapMode::CLAMP_TO_BORDER);
        m_rt.depthBufferTexture()->setBorderColor(Vec4(1.0));
}

void SimpleShadowMapping::updateShadowMap(Light *light, PolygonalList &objects) {
        m_rt.bind();
                Renderer::setCullMode(CullMode::FRONT);
                Renderer::clearBuffers();
                Renderer::setDepthMode(DepthMode::LESS);
                renderObjects(light, objects);
                Renderer::setDepthMode(DepthMode::NONE);
                Renderer::setCullMode(CullMode::BACK);
        m_rt.unbind();

        m_shadowMap = m_rt.depthBufferTexture().get();
}

void SimpleShadowMapping::updateSize(const IVec2 &size) {
        m_rt.setSize(size);
}

//######################### VarianceShadowMapping ############################//

VarianceShadowMapping::VarianceShadowMapping() :
        ShadowTechnique("lights/VSM.glsl", {"VARIANCE_SHADOW_MAPPING"}),
        m_rt1(m_size),
        m_rt2(m_size)
{
        setShadowMapSize(IVec2(128, 128));

        m_rt1.enableColorBuffer(0, InternalFormat::COLOR_FLOAT_3_COMP_32_BIT, true);
        m_rt1.enableDepthBuffer(InternalFormat::DEPTH_NORM_1_COMP_24_BIT, false);
        m_rt1.colorBufferTexture(0)->setWrap(TextureWrapMode::CLAMP_TO_EDGE);

        m_rt2.enableColorBuffer(0, InternalFormat::COLOR_FLOAT_3_COMP_32_BIT, true);
        m_rt2.colorBufferTexture(0)->setWrap(TextureWrapMode::CLAMP_TO_EDGE);

        m_blur.setRadius(10);
}

void VarianceShadowMapping::setSmoothLevel(int radius) {
        m_blur.setRadius(radius);
}

void VarianceShadowMapping::updateShadowMap(Light *light, PolygonalList &objects) {
        m_rt1.bind();
                Renderer::setCullMode(CullMode::BACK);
                Renderer::clearBuffers();
                Renderer::setDepthMode(DepthMode::LESS);
                renderObjects(light, objects);
                Renderer::setDepthMode(DepthMode::NONE);
                Renderer::setCullMode(CullMode::BACK);
        m_rt1.unbind();

        m_rt2.bind();
                m_blur.setDirection(Vec2(1.0, 0.0));
                m_blur.setTexture(m_rt1.colorBufferTexture(0).get());
                m_blur.apply();
        m_rt2.unbind();

        m_rt1.bind();
                m_blur.setDirection(Vec2(0.0, 1.0));
                m_blur.setTexture(m_rt2.colorBufferTexture(0).get());
                m_blur.apply();
        m_rt1.unbind();

        m_shadowMap = m_rt1.colorBufferTexture(0).get();
}

void VarianceShadowMapping::updateSize(const IVec2 &size) {
        m_rt1.setSize(size);
        m_rt2.setSize(size);
}


} //namespace imEngine
