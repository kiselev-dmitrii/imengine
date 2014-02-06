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
        m_box1 = new PolygonalEntity(scene()->world());
        m_box2 = new PolygonalEntity(scene()->world());
        m_box2->setPosition(Vec3(2,2,2));

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
