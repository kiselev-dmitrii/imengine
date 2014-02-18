#include "SceneDeferred.h"

namespace imEngine {

SceneDeferred::SceneDeferred(GraphicApplication* application) :
        Scene(application),
        m_gbuffer(application->window()->size())
{
        initGBuffer();
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

                for (const ModelDetail& detail: object->model().details()) {
                        if (!detail.material->isTransparent()) {
                                detail.material->bind();
                                detail.material->program()->setUniform("uModelViewProjectionMatrix", modelViewProjectionMatrix);
                                detail.material->program()->setUniform("uModelViewMatrix", modelViewMatrix);
                                detail.material->program()->setUniform("uNormalMatrix", normalMatrix);
                                detail.geometry->render();
                        } else {
                                transparentDetails.push_back(& const_cast<ModelDetail&>(detail));
                        }
                }
        }

        m_gbuffer.unbind();
        // Рендер прозрачных деталей
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

} //namespace imEngine
