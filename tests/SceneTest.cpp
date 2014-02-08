#include <imEngine/Application/GraphicApplication.h>
#include <imEngine/Graphics/Scene/Scene.h>
#include <imEngine/Utils/Debug.h>
#include <imEngine/Graphics/Scene/Entity/Polygonal.h>
#include <imEngine/Graphics/GUI/TextButton.h>
#include <imEngine/Graphics/GUI/Panel.h>
#include <imEngine/Graphics/GUI/Slider.h>

using namespace imEngine;

class Application : public GraphicApplication {
protected:
        void    initialize();

        void    keyPressEvent(int key);
private:
        Polygonal*        m_car;
        Polygonal*        m_wheel1;

        CameraAbstract*         m_firstCamera;
        CameraAbstract*         m_secondCamera;

        Panel*                  m_pnl;
        HSlider*                m_slider;
};


void Application::initialize() {
        GraphicApplication::initialize();

        Renderer::setBlendMode(BlendMode::ALPHA);
        Renderer::setDepthMode(DepthMode::LESS);
        m_car = new Polygonal(Model("resources/models/car.xml"), scene()->world());
        m_wheel1 = new Polygonal(Model("resources/models/wheel.xml"), m_car);
        m_wheel1->setPosition(Vec3(1.05,-0.7,0));

        m_firstCamera = scene()->currentCamera();
        m_secondCamera = new FirstPersonCamera(scene()->world());
        scene()->setCurrentCamera(m_secondCamera);

        m_pnl = new Panel("regular_panel.png", gui()->root());
        m_pnl->setOpacity(0.9);
        m_pnl->setPadding(20);
        m_pnl->setSize(Vec2(200, 100));

        m_slider = new HSlider("slider_background.png", "slider_selection.png", "slider_btn_active.png", "slider_btn_hover.png", m_pnl);
        m_slider->setWidth(160);
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
