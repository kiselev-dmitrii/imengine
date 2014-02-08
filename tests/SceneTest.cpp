#include <imEngine/Application/GraphicApplication.h>
#include <imEngine/Graphics/Scene/Scene.h>
#include <imEngine/Utils/Debug.h>
#include <imEngine/Graphics/Scene/Entity/PolygonalEntity.h>

using namespace imEngine;

class Application : public GraphicApplication {
protected:
        void    initialize();

        void    keyPressEvent(int key);
private:
        PolygonalEntity*        m_box1;
        PolygonalEntity*        m_box2;

        CameraAbstract*         m_firstCamera;
        CameraAbstract*         m_secondCamera;
};


void Application::initialize() {
        GraphicApplication::initialize();

        Renderer::setBlendMode(BlendMode::ALPHA);
        Renderer::setDepthMode(DepthMode::LESS);
        m_box1 = new PolygonalEntity(Model("resources/models/car.xml"), scene()->world());

        m_firstCamera = scene()->currentCamera();
        m_secondCamera = new FirstPersonCamera(scene()->world());
        scene()->setCurrentCamera(m_secondCamera);
}

void Application::keyPressEvent(int key) {
        GraphicApplication::keyPressEvent(key);
        if (key == '1') scene()->setCurrentCamera(m_firstCamera);
        if (key == '2') scene()->setCurrentCamera(m_secondCamera);
        if (key == 'p') scene()->currentCamera()->setOrthographic(!scene()->currentCamera()->isOrthographic());
}

int main() {
        Application app;
        return app.exec();
}
