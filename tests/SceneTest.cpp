#include <imEngine/Application/GraphicApplication.h>
#include <imEngine/Graphics/Scene/Scene.h>
#include <imEngine/Utils/Debug.h>
#include <imEngine/Graphics/Scene/Entity/Polygonal.h>
#include <imEngine/Graphics/Scene/Entity/Volume.h>
#include <imEngine/Graphics/GUI/TextButton.h>
#include <imEngine/Graphics/GUI/Panel.h>
#include <imEngine/Graphics/GUI/Slider.h>
#include <imEngine/Graphics/GUI/Text.h>
#include <imEngine/Graphics/GUI/BoxLayout.h>
#include <imEngine/Graphics/Scene/Material/PhongMaterial.h>

using namespace imEngine;

class Application : public GraphicApplication {
protected:
        void    initialize();

        void    keyPressEvent(int key);
private:
        Polygonal*        m_car;
        Polygonal*        m_wheel1;

        Texture3D*              m_data;
        Volume*                 m_volume;

        CameraAbstract*         m_firstCamera;
        CameraAbstract*         m_secondCamera;

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
        m_car = new Polygonal(Model("resources/models/car.xml"), scene()->world());
        m_wheel1 = new Polygonal(Model("resources/models/wheel.xml"), m_car);
        m_wheel1->setPosition(Vec3(1.05,-0.7,0));

        m_data = new Texture3D();
        m_data->load(256,256,128, TextureInternalFormat::COLOR_NORM_1_COMP_8_BIT, TextureSrcType::UBYTE, TextureSrcFormat::R, "resources/texture/engine.raw");
        m_volume = new Volume(m_data, scene()->world());
        m_volume->setPosition(Vec3(3,3,0));

        m_firstCamera = scene()->currentCamera();
        m_secondCamera = new FirstPersonCamera(scene()->world());
        scene()->setCurrentCamera(m_secondCamera);


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
        if (key == '1') scene()->setCurrentCamera(m_firstCamera);
        if (key == '2') scene()->setCurrentCamera(m_secondCamera);
        if (key == 'p') scene()->currentCamera()->setOrthographic(!scene()->currentCamera()->isOrthographic());
        if (key == 'r') {
                m_volume->program()->loadSourceFromFile("resources/shaders/VolumeRender.glsl");
                m_volume->program()->build();
        }
}

int main() {
        Application app;
        return app.exec();
}
