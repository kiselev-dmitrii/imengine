#include "SceneDeferred.h"

namespace imEngine {

SceneDeferred::SceneDeferred(GraphicApplication* application) :
        Scene(application),
        m_gbuffer(application->window()->size()),
        m_lbuffer(application->window()->size())
{
        initGBuffer();
        initLBuffer();
}

SceneDeferred::~SceneDeferred() {

}

RenderTarget* SceneDeferred::gbuffer() {
        return &m_gbuffer;
}

void SceneDeferred::render() {
        Renderer::setDepthMode(DepthMode::LESS);

        const Mat4& viewMatrix = activeCamera()->worldToLocalMatrix();
        const Mat4& projectionMatrix = activeCamera()->viewToClipMatrix();

        m_gbuffer.bind();
        Renderer::clearBuffers();

        // Рендер непрозрачных деталей и поиск прозрачных
        std::list<ModelDetail*> transparentDetails;
        for(Polygonal* object: m_polygonals) {
                const Mat4& modelMatrix = object->localToWorldMatrix();
                Mat4 modelViewMatrix = viewMatrix * modelMatrix;
                Mat4 modelViewProjectionMatrix = projectionMatrix * viewMatrix * modelMatrix;
                Mat3 normalMatrix = glm::transpose(Mat3(glm::inverse(modelViewMatrix)));

                for (ModelDetail& detail: object->model().details()) {
                        if (!detail.material->isTransparent()) {
                                detail.material->bind();
                                detail.material->program()->setUniform("uModelViewProjectionMatrix", modelViewProjectionMatrix);
                                detail.material->program()->setUniform("uModelViewMatrix", modelViewMatrix);
                                detail.material->program()->setUniform("uNormalMatrix", normalMatrix);
                                detail.geometry->render();
                        } else {
                                transparentDetails.push_back(&detail);
                        }
                }
        }

        m_gbuffer.unbind();

        // Рендер прозрачных деталей
        //...

        // Light Pass
        //m_lbuffer.bind();
        Renderer::setDepthMode(DepthMode::NONE);
        glClearColor(0.0,0.0,0.0,0.0);
        Renderer::clearBuffers();
        Renderer::setBlendMode(BlendMode::ADD);
        for (Light* light: m_lights) {
                light->bind(this);
                m_gbuffer.colorBufferTexture(0)->bind(0);
                m_gbuffer.colorBufferTexture(1)->bind(1);
                m_gbuffer.colorBufferTexture(2)->bind(2);
                m_gbuffer.depthBufferTexture()->bind(3);
                light->program()->setUniform("uGBufferDiffuse", 0);
                light->program()->setUniform("uGBufferMaterial", 1);
                light->program()->setUniform("uGBufferGeometry", 2);
                light->program()->setUniform("uGBufferDepth", 3);

                m_quad.render();
                light->unbind();
        }
        //m_lbuffer.unbind();

        /*
        Renderer::setBlendMode(BlendMode::NONE);
        Renderer::clearBuffers();
        m_quad.render();
        */
}

void SceneDeferred::windowResizeEvent(int w, int h) {
        Scene::windowResizeEvent(w, h);
        m_gbuffer.setSize(IVec2(w, h));
}

void SceneDeferred::initGBuffer() {
        m_gbuffer.enableColorBuffer(0, InternalFormat::COLOR_NORM_4_COMP_8_BIT, true);        //GBufferDiffuse
        m_gbuffer.enableColorBuffer(1, InternalFormat::COLOR_NORM_4_COMP_8_BIT, true);        //GBufferMaterial
        m_gbuffer.enableColorBuffer(2, InternalFormat::COLOR_NORM_4_COMP_8_BIT, true);        //GBufferGeometry
        m_gbuffer.enableDepthBuffer(InternalFormat::DEPTH_NORM_1_COMP_24_BIT, true);          //GBufferDepth
}

void SceneDeferred::initLBuffer() {
        m_lbuffer.enableColorBuffer(0, InternalFormat::COLOR_NORM_4_COMP_8_BIT, true);
}


} //namespace imEngine
