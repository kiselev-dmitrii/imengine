#include <imEngine/Application/GraphicApplication.h>
#include <imEngine/Graphics/Scene/Scene.h>
#include <imEngine/Utils/Debug.h>
#include <imEngine/Graphics/Scene/Entity/Polygonal.h>

using namespace imEngine;

class Application : public GraphicApplication {
protected:
        void    initialize();

        void    keyPressEvent(int key);
private:
        Polygonal*        m_car;
        Polygonal*        m_wheel1;
        Polygonal*        m_wheel2;
        Polygonal*        m_wheel3;
        Polygonal*        m_wheel4;

        CameraAbstract*         m_firstCamera;
        CameraAbstract*         m_secondCamera;
};


void Application::initialize() {
        GraphicApplication::initialize();
        glClearColor(0.2,0.2,0.2,1.0);

        Renderer::setBlendMode(BlendMode::ALPHA);
        Renderer::setDepthMode(DepthMode::LESS);
        m_car = new Polygonal(Model("resources/models/car.xml"), scene()->world());
        m_wheel1 = new Polygonal(Model("resources/models/wheel.xml"), m_car);
        m_wheel1->setPosition(Vec3(1.05,-0.7,0));

        /*
        m_wheel2 = new Polygonal(Model("resources/models/wheel.xml"), m_car);
        m_wheel3 = new Polygonal(Model("resources/models/wheel.xml"), m_car);
        m_wheel4 = new Polygonal(Model("resources/models/wheel.xml"), m_car);
        */

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
