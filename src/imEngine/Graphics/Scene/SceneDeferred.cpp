#include "SceneDeferred.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Objects/Light/PointLight.h"
#include "ResourceManager.h"

namespace imEngine {

/*
SceneDeferred::SceneDeferred(GraphicApplication* application) :
        Scene(application),
        m_gbuffer(application->window()->size()),
        m_lbuffer(application->window()->size()),
        m_postEffects(this),
        m_pickedObject(nullptr)
{
        initGBuffer();
        initLBuffer();
        m_skybox = SkyboxPtr(new Skybox("resources/environments/grace.env.hdr"));
}

SceneDeferred::~SceneDeferred() {

}

RenderTarget* SceneDeferred::gbuffer() {
        return &m_gbuffer;
}

PostEffectManager* SceneDeferred::postEffects() {
        return &m_postEffects;
}

void SceneDeferred::render() {
        const Mat4& viewMatrix = activeCamera()->worldToLocalMatrix();
        const Mat4& projectionMatrix = activeCamera()->viewToClipMatrix();

        m_gbuffer.bind();
        Renderer::clearBuffers();

        std::list<ModelDetail*> transparentDetails;
        std::list<ModelDetail*> unlightenedDetails;

        // Рендер непрозрачных деталей
        Renderer::setCullMode(CullMode::BACK);
        Renderer::setDepthMode(DepthMode::LESS);
        Renderer::setBlendMode(BlendMode::NONE);
        for(Polygonal* object: m_polygonals) {
                const Mat4& modelMatrix = object->localToWorldMatrix();
                Mat4 modelViewMatrix = viewMatrix * modelMatrix;
                Mat4 modelViewProjectionMatrix = projectionMatrix * viewMatrix * modelMatrix;
                Mat3 normalMatrix = glm::transpose(Mat3(glm::inverse(modelViewMatrix)));

                for (ModelDetail& detail: object->model().details()) {
                        switch (detail.material->type()) {
                                case MaterialType::DEFERRED:
                                        detail.material->bind();
                                        detail.material->program()->setUniform("uModelViewProjectionMatrix", modelViewProjectionMatrix);
                                        detail.material->program()->setUniform("uModelViewMatrix", modelViewMatrix);
                                        detail.material->program()->setUniform("uNormalMatrix", normalMatrix);
                                        detail.geometry->render();
                                        break;

                                case MaterialType::TRANSPARENT:
                                        transparentDetails.push_back(&detail);
                                        break;
                                case MaterialType::UNLIGHTENED:
                                        unlightenedDetails.push_back(&detail);
                                        break;
                        }
                }
        }

        m_gbuffer.unbind();

        // Рендер прозрачных деталей
        //...

        // Рендер скайбокса
        //m_skybox->render(Mat3(viewMatrix), projectionMatrix);

        // Рендер теневых карт
        for (Light* light: m_lights) {
                light->calculateShadowMap();
        }


        // Light Pass
        m_lbuffer.bind();
        Renderer::setDepthMode(DepthMode::NONE);
        glClearColor(0.0,0.0,0.0,0.0);
        Renderer::clearBuffers(Buffer::COLOR);
        Renderer::setBlendMode(BlendMode::ADD);


        for (Light* light: m_lights) {
                light->setDiffuseBuffer(m_gbuffer.colorBufferTexture(0).get());
                light->setMaterialBuffer(m_gbuffer.colorBufferTexture(1).get());
                light->setGeometryBuffer(m_gbuffer.colorBufferTexture(2).get());
                light->setDepthBuffer(m_gbuffer.depthBufferTexture().get());
                light->apply();
        }
        m_lbuffer.unbind();

        // Unlightened object pass
        m_lbuffer.setDepthBuffer(m_gbuffer.depthBufferTexture());
        m_lbuffer.bind();
        Renderer::setDepthMode(DepthMode::LESS);
        for (ModelDetail* detail: unlightenedDetails) {
                const Mat4& modelMatrix = detail->owner->owner()->localToWorldMatrix();
                Mat4 modelViewMatrix = viewMatrix * modelMatrix;
                Mat4 modelViewProjectionMatrix = projectionMatrix * viewMatrix * modelMatrix;
                Mat3 normalMatrix = glm::transpose(Mat3(glm::inverse(modelViewMatrix)));
                detail->material->bind();
                detail->material->program()->setUniform("uModelViewProjectionMatrix", modelViewProjectionMatrix);
                detail->material->program()->setUniform("uModelViewMatrix", modelViewMatrix);
                detail->material->program()->setUniform("uNormalMatrix", normalMatrix);
                detail->geometry->render();
        }
        Renderer::setDepthMode(DepthMode::NONE);
        m_lbuffer.unbind();
        m_lbuffer.disableDepthBuffer();

        Renderer::setBlendMode(BlendMode::NONE);
        Renderer::clearBuffers();

        m_postEffects.apply(m_lbuffer.colorBufferTexture(0).get(),
                            m_gbuffer.colorBufferTexture(0).get(),
                            m_gbuffer.colorBufferTexture(1).get(),
                            m_gbuffer.colorBufferTexture(2).get(),
                            m_gbuffer.depthBufferTexture().get());
}

void SceneDeferred::windowResizeEvent(int w, int h) {
        Scene::windowResizeEvent(w, h);
        m_gbuffer.setSize(IVec2(w, h));
}

void SceneDeferred::initGBuffer() {
        m_gbuffer.enableColorBuffer(0, InternalFormat::COLOR_NORM_4_COMP_8_BIT, true);        //GBufferDiffuse
        m_gbuffer.enableColorBuffer(1, InternalFormat::COLOR_NORM_4_COMP_8_BIT, true);        //GBufferMaterial
        m_gbuffer.enableColorBuffer(2, InternalFormat::COLOR_FLOAT_4_COMP_16_BIT, true);      //GBufferGeometry
        m_gbuffer.enableDepthBuffer(InternalFormat::DEPTH_NORM_1_COMP_24_BIT, true);          //GBufferDepth

        m_gbuffer.colorBufferTexture(0)->setWrap(TextureWrapMode::CLAMP_TO_EDGE);
        m_gbuffer.colorBufferTexture(1)->setWrap(TextureWrapMode::CLAMP_TO_EDGE);
        m_gbuffer.colorBufferTexture(2)->setWrap(TextureWrapMode::CLAMP_TO_EDGE);
        m_gbuffer.depthBufferTexture()->setWrap(TextureWrapMode::CLAMP_TO_EDGE);
}

void SceneDeferred::initLBuffer() {
        m_lbuffer.enableColorBuffer(0, InternalFormat::COLOR_NORM_4_COMP_8_BIT, true);
}
*/

} //namespace imEngine
