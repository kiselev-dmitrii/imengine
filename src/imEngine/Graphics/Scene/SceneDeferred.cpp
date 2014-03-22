#include "SceneDeferred.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Objects/Light/PointLight.h"
#include "ResourceManager.h"
#include "PostEffects/Passes/BlitCopyPass.h"

namespace imEngine {

SceneDeferred::SceneDeferred(GraphicApplication* application) :
        Scene(application),
        m_gbuffer(application->window()->size()),
        m_lbuffer(application->window()->size()),
        m_pickedObject(nullptr)
{
        initGBuffer();
        initLBuffer();
        m_skybox = SkyboxPtr(new Skybox("resources/environments/grace.env.hdr"));
}

SceneDeferred::~SceneDeferred() {

}

Polygonal* SceneDeferred::pickObject(int x, int y) {
        IVec2 size = application()->window()->size();
        const Mat4& viewMatrix = activeCamera()->worldToLocalMatrix();
        const Mat4& projectionMatrix = activeCamera()->viewToClipMatrix();

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

        std::list<ModelDetail*> transparentDetails;
        std::list<ModelDetail*> unlightenedDetails;

        // Рендер непрозрачных деталей
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

        // Light Pass
        m_lbuffer.bind();
        Renderer::setDepthMode(DepthMode::NONE);
        glClearColor(0.0,0.0,0.0,0.0);
        Renderer::clearBuffers(Buffer::COLOR);
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

        // Unlightened object pass
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

        m_lbuffer.unbind();


        static BlitCopyPass copy(m_lbuffer.colorBufferTexture(0).get());
        Renderer::setBlendMode(BlendMode::NONE);
        Renderer::clearBuffers();
        copy.apply();
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

void SceneDeferred::initGBuffer() {
        m_gbuffer.enableColorBuffer(0, InternalFormat::COLOR_NORM_4_COMP_8_BIT, true);        //GBufferDiffuse
        m_gbuffer.enableColorBuffer(1, InternalFormat::COLOR_NORM_4_COMP_8_BIT, true);        //GBufferMaterial
        m_gbuffer.enableColorBuffer(2, InternalFormat::COLOR_FLOAT_4_COMP_16_BIT, true);      //GBufferGeometry
        m_gbuffer.enableDepthBuffer(InternalFormat::DEPTH_NORM_1_COMP_24_BIT, true);          //GBufferDepth
}

void SceneDeferred::initLBuffer() {
        m_lbuffer.enableColorBuffer(0, InternalFormat::COLOR_NORM_4_COMP_8_BIT, true);
}


} //namespace imEngine
