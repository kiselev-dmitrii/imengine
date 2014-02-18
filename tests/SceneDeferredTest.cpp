#include <imEngine/Graphics/Scene/Scene.h>
#include <imEngine/Utils/Debug.h>
#include <imEngine/Graphics/GUI/Picture.h>
#include <imEngine/Graphics/Scene/Objects/Camera/FirstPersonCamera.h>
#include <imEngine/Graphics/Scene/SceneDeferred.h>

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
};


void Application::initialize() {
        GraphicApplication::initialize();
        SceneDeferred* dscene = (SceneDeferred*) scene();

        m_car = new Polygonal("resources/models/car.xml", scene()->world());

        m_diffuseBuffer = new PictureColor(dscene->gbuffer()->colorBufferTexture(0), gui()->root());
        m_diffuseBuffer->setSize(Vec2(window()->size())/2.0f);
        m_diffuseBuffer->setPosition(Vec2(0,0));

        m_materialBuffer = new PictureColor(dscene->gbuffer()->colorBufferTexture(1), gui()->root());
        m_materialBuffer->setSize(Vec2(window()->size())/2.0f);
        m_materialBuffer->setPosition(Vec2(m_diffuseBuffer->width(),0));

        m_geometryBuffer = new PictureColor(dscene->gbuffer()->colorBufferTexture(2), gui()->root());
        m_geometryBuffer->setSize(Vec2(window()->size())/2.0f);
        m_geometryBuffer->setPosition(Vec2(0,m_diffuseBuffer->height()));

        m_depthBuffer = new PictureDepth(dscene->gbuffer()->depthBufferTexture(), gui()->root());
        m_depthBuffer->setSize(Vec2(window()->size())/2.0f);
        m_depthBuffer->setPosition(m_diffuseBuffer->size());

}

void Application::keyPressEvent(int key) {
        GraphicApplication::keyPressEvent(key);
}

void Application::windowResizeEvent(int x, int y) {
        GraphicApplication::windowResizeEvent(x, y);

        m_diffuseBuffer->setSize(Vec2(window()->size())/2.0f);
        m_diffuseBuffer->setPosition(Vec2(0,0));
        m_materialBuffer->setSize(Vec2(window()->size())/2.0f);
        m_materialBuffer->setPosition(Vec2(m_diffuseBuffer->width(),0));
        m_geometryBuffer->setSize(Vec2(window()->size())/2.0f);
        m_geometryBuffer->setPosition(Vec2(0,m_diffuseBuffer->height()));
        m_depthBuffer->setSize(Vec2(window()->size())/2.0f);
        m_depthBuffer->setPosition(m_diffuseBuffer->size());
}

int main() {
        Application app;
        return app.exec();
}
