#include <imEngine/Application/GraphicApplication.h>
#include <imEngine/Graphics/Scene/Scene.h>
#include <imEngine/Utils/Debug.h>
#include <imEngine/Graphics/GUI/TextButton.h>
#include <imEngine/Graphics/GUI/Panel.h>
#include <imEngine/Graphics/GUI/Slider.h>
#include <imEngine/Graphics/GUI/Text.h>
#include <imEngine/Graphics/GUI/BoxLayout.h>
#include <imEngine/Graphics/Scene/Objects/Camera/FirstPersonCamera.h>
#include <sstream>
#include <iomanip>

using namespace imEngine;

class Application : public GraphicApplication {
protected:
        void    initialize();

        void    keyPressEvent(int key);
private:
        Entity*        m_car;
        Entity*        m_wheel1;

        Texture3D*              m_engine;
        Texture3D*              m_head;

        Volume*                 m_headVolume;
        Volume*                 m_engineVolume;

        Camera*         m_firstCamera;
        Camera*         m_secondCamera;

        Panel*                  m_pnl;
        VBoxLayout*             m_diffuseLayout;
        HSlider*                m_redSlider;
        HSlider*                m_greenSlider;
        HSlider*                m_blueSlider;

        HSlider*                m_shininessSlider;
};


void Application::initialize() {
        GraphicApplication::initialize();

        Renderer::setBlendMode(BlendMode::ALPHA);
        Renderer::setDepthMode(DepthMode::LESS);
        m_car = new Entity("resources/models/car.xml", scene()->world());
        m_wheel1 = new Entity("resources/models/wheel.xml", m_car);
        m_wheel1->setPosition(Vec3(1.05,-0.7,0));

        /// Грузим текстуру головы из dcm файлов
        m_head = new Texture3D();
        StringList filenames;
        for (uint i = 1; i <= 245; ++i) {
                std::stringstream ss;
                ss << "vhm.1" << std::setw(3) << std::setfill('0') << i << ".dcm";
                filenames.push_back("resources/texture/head/" + ss.str());
        }
        m_head->load(filenames);

        /// Грузим текстуру двигателя из raw
        m_engine = new Texture3D();
        m_engine->load(256,256,128, InternalFormat::COLOR_NORM_1_COMP_8_BIT, SourceType::UBYTE, SourceFormat::R, "resources/texture/engine.raw");

        /*
        /// Создаем Volume объекты
        m_headVolume = new Volume(m_head, scene()->world());
        m_headVolume->setPosition(Vec3(3,3,0));

        m_engineVolume = new Volume(m_engine, scene()->world());
        m_engineVolume->setPosition(Vec3(4,5,0));
        */

        m_firstCamera = scene()->activeCamera();
        m_secondCamera = new FirstPersonCamera(scene()->world());
        scene()->setActiveCamera(m_secondCamera);


        /*
        m_pnl = new Panel("regular_panel.png", gui()->root());
        m_pnl->setOpacity(0.9);
        m_pnl->setPadding(20);
        m_pnl->setSize(Vec2(200, 300));

        m_diffuseLayout = new VBoxLayout(m_pnl);
        m_redSlider = new HSlider("slider_background.png", "slider_selection.png", "slider_btn_active.png", "slider_btn_hover.png", m_diffuseLayout);
        m_redSlider->setWidth(m_pnl->contentWidth());
        m_greenSlider = new HSlider("slider_background.png", "slider_selection.png", "slider_btn_active.png", "slider_btn_hover.png", m_diffuseLayout);
        m_greenSlider->setWidth(m_pnl->contentWidth());
        m_blueSlider = new HSlider("slider_background.png", "slider_selection.png", "slider_btn_active.png", "slider_btn_hover.png", m_diffuseLayout);
        m_blueSlider->setWidth(m_pnl->contentWidth());

        m_shininessSlider = new HSlider("slider_background.png", "slider_selection.png", "slider_btn_active.png", "slider_btn_hover.png", m_diffuseLayout);
        m_shininessSlider->setMinMaxValues(0, 120);
        m_shininessSlider->setWidth(m_pnl->contentWidth());

        m_diffuseLayout->addWidget(new Text("Diffuse color", m_diffuseLayout));
        m_diffuseLayout->addWidget(m_redSlider);
        m_diffuseLayout->addWidget(m_greenSlider);
        m_diffuseLayout->addWidget(m_blueSlider);
        m_diffuseLayout->addSpacing(20);
        m_diffuseLayout->addWidget(new Text("Shininess", m_diffuseLayout));
        m_diffuseLayout->addWidget(m_shininessSlider);

        auto colorSetter = [&] (HSlider* slider) {
                Vec3 color;
                float shininess;
                color.x = m_redSlider->value();
                color.y = m_greenSlider->value();
                color.z = m_blueSlider->value();
                shininess = m_shininessSlider->value();

                PhongMaterial* material = (PhongMaterial*)(m_car->model()->details()[0].material.get());
                if (material != 0) {
                        material->setDiffuseColor(color);
                        material->setShininess(shininess);
                }
        };

        m_redSlider->onValueChanged += colorSetter;
        m_greenSlider->onValueChanged += colorSetter;
        m_blueSlider->onValueChanged += colorSetter;
        m_shininessSlider->onValueChanged += colorSetter;
        */

}

void Application::keyPressEvent(int key) {
        GraphicApplication::keyPressEvent(key);
        if (key == '1') scene()->setActiveCamera(m_firstCamera);
        if (key == '2') scene()->setActiveCamera(m_secondCamera);
        if (key == 'p') scene()->activeCamera()->setOrthographic(!scene()->activeCamera()->isOrthographic());
        if (key == 'r') {
                //m_engineVolume->program()->loadSourceFromFile("resources/shaders/VolumeRender.glsl");
                //m_engineVolume->program()->build();
        }
}

int main() {
        Application app;
        return app.exec();
}
