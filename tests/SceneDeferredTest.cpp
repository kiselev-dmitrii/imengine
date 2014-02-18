#include <imEngine/Graphics/Scene/Scene.h>
#include <imEngine/Utils/Debug.h>
#include <imEngine/Graphics/GUI/Picture.h>
#include <imEngine/Graphics/Scene/Objects/Camera/FirstPersonCamera.h>
#include <imEngine/Graphics/Scene/SceneDeferred.h>
#include <imEngine/Graphics/Scene/Objects/Light/PointLight.h>

using namespace imEngine;

class Application : public GraphicApplication {
protected:
        void    initialize();
        void    keyPressEvent(int key);
        void    windowResizeEvent(int x, int y);

private:
        Polygonal*      m_car;
        PictureColor*   m_diffuseBuffer;
        PictureColor*   m_materialBuffer;
        PictureColor*   m_geometryBuffer;
        PictureDepth*   m_depthBuffer;

        PointLight*     m_light1;
        PointLight*     m_light2;
};


void Application::initialize() {
        GraphicApplication::initialize();
        SceneDeferred* dscene = (SceneDeferred*) scene();

        m_car = new Polygonal("resources/models/car.xml", scene()->world());
        m_light1 = new PointLight(scene()->world());
        m_light1->setPosition(Vec3(10,10,10));
        m_light2 = new PointLight(scene()->world());
        m_light2->setPosition(Vec3(-10, -10, 10));
        m_light2->setColor(Vec3(0.5, 0.5, 1.0));
        m_light1->setColor(Vec3(0.5, 1.0, 0.5));

        m_diffuseBuffer = new PictureColor(dscene->gbuffer()->colorBufferTexture(0), gui()->root());
        m_materialBuffer = new PictureColor(dscene->gbuffer()->colorBufferTexture(1), gui()->root());
        m_geometryBuffer = new PictureColor(dscene->gbuffer()->colorBufferTexture(2), gui()->root());
        m_depthBuffer = new PictureDepth(dscene->gbuffer()->depthBufferTexture(), gui()->root());
        windowResizeEvent(window()->size().x, window()->size().y);

}

void Application::keyPressEvent(int key) {
        GraphicApplication::keyPressEvent(key);
}

void Application::windowResizeEvent(int x, int y) {
        GraphicApplication::windowResizeEvent(x, y);

        m_diffuseBuffer->setSize(Vec2(window()->size())/8.0f);
        m_diffuseBuffer->setPosition(Vec2(0,0));
        m_materialBuffer->setSize(Vec2(window()->size())/8.0f);
        m_materialBuffer->setPosition(Vec2(m_diffuseBuffer->width(),0));
        m_geometryBuffer->setSize(Vec2(window()->size())/8.0f);
        m_geometryBuffer->setPosition(Vec2(2*m_diffuseBuffer->width(),0));
        m_depthBuffer->setSize(Vec2(window()->size())/8.0f);
        m_depthBuffer->setPosition(Vec2(3*m_diffuseBuffer->width(),0));
}

int main() {
        Application app;
        return app.exec();
}
