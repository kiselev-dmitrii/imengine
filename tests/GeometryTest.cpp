#include <imEngine/Application/GraphicApplication.h>
#include <imEngine/FileContainers/Mesh.h>
#include <imEngine/Graphics/Geometry.h>
#include <imEngine/Utils/Debug.h>
#include <imEngine/Graphics/Scene/SceneObject.h>
#include <imEngine/Graphics/Scene/Camera.h>
#include <glm/gtx/transform.hpp>
#include "showGeometry.glsl"
#include <sstream>

using namespace imEngine;

class Application : public GraphicApplication {
protected:
        void    initialize();
        void    update();
        void    render();
        void    destroy();

        void    windowResizeEvent(int x, int y);

private:
        Geometry*               m_geo;
        GeometryPtr             m_box;
        ProgramPtr              m_program;
        SceneObject*                m_world;
        SceneObject*                m_object;
        FirstPersonCamera*      m_camera;
        SceneObject*                m_obj2;
};


void Application::initialize() {
        GraphicApplication::initialize();
        /*
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        */
        Renderer::setBlendMode(BlendMode::ALPHA);

        m_world = new SceneObject(nullptr);
        m_object = new SceneObject(m_world);

        m_camera = new FirstPersonCamera(m_world);
        m_obj2 = new FirstPersonCamera(m_world);
        m_camera->attachChild(m_obj2);
        m_obj2->setPosition(Vec3(0,0,-4));

        Mesh mesh("resources/models/cube.obj");
        m_geo = new Geometry(mesh);

        m_program = ProgramPtr(new Program());
        m_program->loadSource(showGeometrySource);
        m_program->build();

        m_box = Geometry::cube();
        /*
        Mat4 invProjectionMatrix = glm::inverse(m_camera->viewToClipMatrix());
        m_box->transform(invProjectionMatrix);
        */
}

void Application::update() {
        GraphicApplication::update();
        m_camera->update(0.001);
}

void Application::render() {
        GraphicApplication::render();

        Mat4 modelMatrix = m_object->localToWorldMatrix();
        Mat4 viewMatrix = m_camera->worldToLocalMatrix();
        Mat4 projectionMatrix = m_camera->viewToClipMatrix();

        Mat4 modelViewProjectionMatrix = projectionMatrix * viewMatrix * modelMatrix;
        m_program->bind();
        m_program->setUniform("uModelViewProjectionMatrix", modelViewProjectionMatrix);
        //m_geo->render();

        //modelViewProjectionMatrix = projectionMatrix;
        //m_program->setUniform("uModelViewProjectionMatrix", modelViewProjectionMatrix);

        Renderer::setPolygonMode(PolygonMode::LINE);
        m_box->render();
        Renderer::setPolygonMode(PolygonMode::FILL);
}

void Application::destroy() {
        GraphicApplication::destroy();
}

void Application::windowResizeEvent(int x, int y) {
        GraphicApplication::windowResizeEvent(x, y);
        m_camera->setAspectRatio(float(x)/y);
}

int main() {
        Application app;
        return app.exec();
}
