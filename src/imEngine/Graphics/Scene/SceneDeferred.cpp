#include "SceneDeferred.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Objects/Light/PointLight.h"
#include "ResourceManager.h"

namespace imEngine {

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

Polygonal* SceneDeferred::pickObject(int x, int y) {

        float aspectRatio = activeCamera()->aspectRatio();
        float tanHalfFovy = glm::tan(glm::radians(activeCamera()->fieldOfView()/2));

        /// Приводим мышь к [-1;1]
        Vec2 m = Vec2(x,600-y)/Vec2(800, 600);
        m = 2.0f * m - Vec2(1.0);

        Vec3 vViewRay = Vec3(
                m.x * aspectRatio * tanHalfFovy,
                m.y * tanHalfFovy,
                -1
        );
        vViewRay = glm::normalize(vViewRay);

        Vec3 cameraForwardWS = activeCamera()->convertLocalToWorld(vViewRay) - activeCamera()->worldPosition();
        Vec3 positionWS = activeCamera()->worldPosition();


        /// Исключить объект, в котором мы сейчас находимся
        Polygonal* exclude = nullptr;
        for (Polygonal* polygonal: m_polygonals) {
                const Mat4& invModelMatrix = polygonal->worldToLocalMatrix();
                Vec3 modelSpacePosition = Vec3(invModelMatrix * Vec4(positionWS, 1.0));
                if (polygonal->aabb().doesContain(modelSpacePosition)) exclude = polygonal;
        }

        while (glm::length(positionWS - cameraForwardWS) < 20.0) {
                for (Polygonal* polygonal: m_polygonals) {
                        if (polygonal == exclude) continue;
                        const Mat4& invModelMatrix = polygonal->worldToLocalMatrix();
                        Vec3 modelSpacePosition = Vec3(invModelMatrix * Vec4(positionWS, 1.0));
                        if (polygonal->aabb().doesContain(modelSpacePosition)) return polygonal;
                }
                positionWS += 0.1f * cameraForwardWS;
        }
        return nullptr;

        /*
        /// Получаем глубину
        float depth;
        m_gbuffer.bind();
                 glReadPixels(x, size.y-y-1, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
        m_gbuffer.unbind();
        Vec3 winSpacePosition(x, size.y-y-1, depth);

        /// Получаем мировые координаты
        Vec3 worldSpacePosition = glm::unProject(winSpacePosition, viewMatrix, projectionMatrix, Vec4(0,0, size));

        /// Проходим по всем объектам и смотрим принадлежность точки boundBox
        for (Polygonal* polygonal: m_polygonals) {
                const Mat4& invModelMatrix = polygonal->worldToLocalMatrix();
                Vec3 modelSpacePosition = Vec3(invModelMatrix * Vec4(worldSpacePosition, 1.0));

                if (polygonal->aabb().doesContain(modelSpacePosition)) return polygonal;
        }
        return nullptr;
        */

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

void SceneDeferred::mousePressEvent(int x, int y, char button) {
        Scene::mousePressEvent(x, y, button);
        if (button != MouseButton::RIGHT) return;

        m_pickedObject = pickObject(x, y);
}

void SceneDeferred::mouseMoveEvent(int oldX, int oldY, int newX, int newY) {
        if (m_pickedObject) {
                Vec2 delta = Vec2(newX, newY) - Vec2(oldX, oldY);
                Vec2 angles = delta;

                Vec3 worldSpaceCameraUp = activeCamera()->convertLocalToWorld(Vec3(0,1,0)) - activeCamera()->worldPosition();
                Vec3 worldSpaceCameraRight = activeCamera()->convertLocalToWorld(Vec3(1,0,0)) - activeCamera()->worldPosition();
                m_pickedObject->rotate(worldSpaceCameraUp, angles.x, Space::WORLD);
                m_pickedObject->rotate(worldSpaceCameraRight, angles.y, Space::WORLD);
        }
}

void SceneDeferred::mouseReleaseEvent(int x, int y, char button) {
        Scene::mouseReleaseEvent(x, y, button);
        if (button != MouseButton::RIGHT) return;

        m_pickedObject = nullptr;
}

void SceneDeferred::keyPressEvent(int key) {
        /*
        static int step = 1;
        static int radius = 0.0;
        if (key == SDLK_LEFT) --step;
        if (key == SDLK_RIGHT) ++step;
        m_pass.setStep(step);

        if (key == '-') m_pass. setRadius(100);
        if (key == '=') m_pass.setRadius(0);
        IM_VAR(step);
        IM_VAR(radius);
        */
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


} //namespace imEngine
