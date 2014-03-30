#include "SceneRenderer.h"

namespace imEngine {

SceneRenderer::SceneRenderer(Scene *scene) :
        m_scene(scene),
        m_gBuffer(Renderer::viewportSize()),
        m_lightAccum(Renderer::viewportSize())
{
        initGBuffer();
        initLightAccum();
}

void SceneRenderer::render() {
        /// Разделяем детали на 3 списка
        ModelDetailPtrList      regularDetails;
        ModelDetailPtrList      unlightenedDetails;
        ModelDetailPtrList      transparentDetails;

        groupDetails(m_scene->polygonals(), &regularDetails, &unlightenedDetails, &transparentDetails);

        /// Подготовливаем видовую и проекционную матрицы
        const Mat4& viewMatrix = m_scene->activeCamera()->worldToLocalMatrix();
        const Mat4& projectionMatrix = m_scene->activeCamera()->viewToClipMatrix();

        renderRegularDetails(regularDetails, viewMatrix, projectionMatrix);
        calculateLighting(m_scene->ligths());
        applySSAO(lightAccum()->colorBufferTexture(0).get(),
                  m_gBuffer.colorBufferTexture(2).get(),
                  m_gBuffer.depthBufferTexture().get());
        renderUnlightenedDetails(unlightenedDetails, viewMatrix, projectionMatrix);
        applyPostEffects(m_lightAccum.colorBufferTexture(0).get(),
                            m_gBuffer.colorBufferTexture(0).get(),
                            m_gBuffer.colorBufferTexture(1).get(),
                            m_gBuffer.colorBufferTexture(2).get(),
                            m_gBuffer.depthBufferTexture().get());
}

void SceneRenderer::windowResizeEvent(int w, int h) {
        m_gBuffer.setSize(IVec2(w, h));
        m_lightAccum.setSize(IVec2(w, h));
}

void SceneRenderer::initGBuffer() {
        m_gBuffer.enableColorBuffer(0, InternalFormat::COLOR_NORM_4_COMP_8_BIT, true);        //GBufferDiffuse
        m_gBuffer.enableColorBuffer(1, InternalFormat::COLOR_NORM_4_COMP_8_BIT, true);        //GBufferMaterial
        m_gBuffer.enableColorBuffer(2, InternalFormat::COLOR_FLOAT_4_COMP_16_BIT, true);      //GBufferGeometry
        m_gBuffer.enableDepthBuffer(InternalFormat::DEPTH_NORM_1_COMP_24_BIT, true);          //GBufferDepth

        m_gBuffer.colorBufferTexture(0)->setWrap(TextureWrapMode::CLAMP_TO_EDGE);
        m_gBuffer.colorBufferTexture(1)->setWrap(TextureWrapMode::CLAMP_TO_EDGE);
        m_gBuffer.colorBufferTexture(2)->setWrap(TextureWrapMode::CLAMP_TO_EDGE);
        m_gBuffer.depthBufferTexture()->setWrap(TextureWrapMode::CLAMP_TO_EDGE);
}

void SceneRenderer::initLightAccum() {
        m_lightAccum.enableColorBuffer(0, InternalFormat::COLOR_NORM_4_COMP_8_BIT, true);
        m_lightAccum.colorBufferTexture(0)->setWrap(TextureWrapMode::CLAMP_TO_EDGE);
}

void SceneRenderer::groupDetails(const PolygonalList &objects, ModelDetailPtrList *outRegular, ModelDetailPtrList *outUnlightnment, ModelDetailPtrList *outTransparent) {
        for (Polygonal* object: objects) {
                for (ModelDetail& detail: object->model().details()) {
                        switch (detail.material->type()) {
                                case MaterialType::DEFERRED:
                                        outRegular->push_back(&detail);
                                        break;
                                case MaterialType::UNLIGHTENED:
                                        outUnlightnment->push_back(&detail);
                                        break;
                                case MaterialType::TRANSPARENT:
                                        outTransparent->push_back(&detail);
                                        break;
                        }
                }
        }
}

void SceneRenderer::renderRegularDetails(const ModelDetailPtrList &details, const Mat4 &viewMatrix, const Mat4 &projectionMatrix) {
        m_gBuffer.bind();
                Renderer::setCullMode(CullMode::BACK);
                Renderer::setDepthMode(DepthMode::LESS);
                Renderer::setBlendMode(BlendMode::NONE);

                Renderer::clearBuffers();
                for (ModelDetail* detail: details) {
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
        m_gBuffer.unbind();
}

void SceneRenderer::calculateLighting(const LightList &lights) {
        // Рассчитываем теневуе карты
        for (Light* light: lights) light->calculateShadowMap();

        // Накапливаем свет в буфере
        m_lightAccum.bind();
                glClearColor(0,0,0,0);
                Renderer::setDepthMode(DepthMode::NONE);
                Renderer::setBlendMode(BlendMode::ADD);

                Renderer::clearBuffers(Buffer::COLOR);
                for (Light* light: lights) {
                        light->setDiffuseBuffer(m_gBuffer.colorBufferTexture(0).get());
                        light->setMaterialBuffer(m_gBuffer.colorBufferTexture(1).get());
                        light->setGeometryBuffer(m_gBuffer.colorBufferTexture(2).get());
                        light->setDepthBuffer(m_gBuffer.depthBufferTexture().get());
                        light->apply();
                }
        m_lightAccum.unbind();
}

void SceneRenderer::renderUnlightenedDetails(const ModelDetailPtrList &details, const Mat4 &viewMatrix, const Mat4 &projectionMatrix) {
        // Подключаем к lightAccum буфер глубины и рендерим в него наши обхекты
        m_lightAccum.setDepthBuffer(m_gBuffer.depthBufferTexture());
        m_lightAccum.bind();
                Renderer::setCullMode(CullMode::BACK);
                Renderer::setDepthMode(DepthMode::LESS);
                Renderer::setBlendMode(BlendMode::NONE);

                for (ModelDetail* detail: details) {
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
        m_lightAccum.unbind();
        m_lightAccum.disableDepthBuffer();
}

void SceneRenderer::applySSAO(Texture2D *lightAccum, Texture2D *geometryBuffer, Texture2D *depthBuffer) {
        if (m_ssao.isEnabled()) {
                m_ssao.setInputTexture(lightAccum);
                m_ssao.setNormalTexture(geometryBuffer);
                m_ssao.setDepthTexture(depthBuffer);
                m_ssao.setActiveCamera(m_scene->activeCamera());
                Texture2D* result = m_ssao.apply();

                m_lightAccum.bind();
                        m_copy.setTexture(result);
                        m_copy.apply();
                m_lightAccum.unbind();
        }

}

void SceneRenderer::applyPostEffects(Texture2D *lightAccum, Texture2D *diffuseBuffer, Texture2D *materialBuffer, Texture2D *geometryBuffer, Texture2D *depthBuffer) {
        Renderer::setBlendMode(BlendMode::NONE);
        Renderer::clearBuffers();

        Texture2D* result = lightAccum;

        if (m_dof.isEnabled()) {
                m_dof.setInputTexture(result);
                m_dof.setDepthTexture(depthBuffer);
                m_dof.setActiveCamera(m_scene->activeCamera());
                result = m_dof.apply();
        }

        if (m_bloom.isEnabled()) {
                m_bloom.setTexture(result);
                result = m_bloom.apply();
        }

        m_copy.setTexture(result);
        m_copy.apply();
}

} //namespace imEngine
