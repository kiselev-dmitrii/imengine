#include <imEngine/Application/GraphicApplication.h>
#include <imEngine/FileContainers/Mesh.h>
#include <imEngine/Graphics/Scene/Geometry.h>
#include <imEngine/Utils/Debug.h>
#include <imEngine/Graphics/Scene/Movable.h>
#include <imEngine/Graphics/Scene/Camera.h>
#include <glm/gtx/transform.hpp>
#include "showGeometry.glsl"

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
        Geometry*               m_box;
        ProgramPtr              m_program;
        Movable*                m_world;
        Movable*                m_object;
        FirstPersonCamera*      m_camera;
        Movable*                m_obj2;
};


void Application::initialize() {
        GraphicApplication::initialize();
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        m_world = new Movable();
        m_object = new Movable(m_world);

        m_camera = new FirstPersonCamera(m_world);
        m_obj2 = new FirstPersonCamera();
        m_camera->attachChild(m_obj2);
        m_obj2->setPosition(Vec3(0,0,-4));

        Mesh mesh("resources/models/cube.obj");
        m_geo = new Geometry(mesh);
        m_box = new Geometry(mesh);

        m_box->transform(glm::translate(Mat4(1.0), Vec3(-m_box->center())));
        m_box->transform(glm::scale(Mat4(1.0), Vec3(2,2,2)));
        IM_VAR(m_box->center());
        IM_VAR(m_box->aabb().min);
        IM_VAR(m_box->aabb().max);

        IM_VAR(m_geo->aabb().max);
        IM_VAR(m_geo->aabb().min);
        IM_VAR(m_geo->radius());
        IM_VAR(m_geo->center());

        m_program = ProgramPtr(new Program());
        m_program->loadSource(showGeometrySource);
        m_program->build();


        Mat4 invProjectionMatrix = glm::inverse(m_camera->viewToClipMatrix());
        IM_VAR(glm::normalize(invProjectionMatrix * Vec4(1,1,1,1)));
        IM_VAR(glm::normalize(invProjectionMatrix * Vec4(1,1,-1,1)));
        IM_VAR(glm::normalize(invProjectionMatrix * Vec4(1,-1,1,1)));
        IM_VAR(glm::normalize(invProjectionMatrix * Vec4(1,-1,-1,1)));
        IM_VAR(glm::normalize(invProjectionMatrix * Vec4(-1,1,1,1)));
        IM_VAR(glm::normalize(invProjectionMatrix * Vec4(-1,1,-1,1)));
        IM_VAR(glm::normalize(invProjectionMatrix * Vec4(-1,-1,1,1)));
        IM_VAR(glm::normalize(invProjectionMatrix * Vec4(-1,-1,-1,1)));
}

void Application::update() {
        GraphicApplication::update();
        m_camera->update(this, 0.001);
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

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        m_box->render();
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


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
