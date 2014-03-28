#include <imEngine/Graphics/Scene/Scene.h>
#include <imEngine/Utils/Debug.h>
#include <imEngine/Graphics/GUI/Picture.h>
#include <imEngine/Graphics/Scene/Objects/Camera/FirstPersonCamera.h>
#include <imEngine/Graphics/Scene/SceneDeferred.h>
#include <imEngine/Graphics/Scene/Objects/Light/PointLight.h>
#include <imEngine/Graphics/Scene/Objects/Light/SpotLight.h>
#include <imEngine/Graphics/Scene/Materials/EmissiveMaterial.h>

#include <imEngine/Graphics/GUI/Panel.h>
#include <imEngine/Graphics/GUI/Slider.h>
#include <imEngine/Graphics/GUI/BoxLayout.h>

using namespace imEngine;

class Application : public GraphicApplication {
protected:
        void    initialize();
        void    keyPressEvent(int key);
        void    windowResizeEvent(int x, int y);

private:
        Polygonal*      m_room;
        Polygonal*      m_logo;
        PictureColor*   m_diffuseBuffer;
        PictureColor*   m_materialBuffer;
        PictureColor*   m_geometryBuffer;
        PictureDepth*   m_depthBuffer;

        PointLight*     m_light1;
        SpotLight*      m_light2;
        Polygonal*      m_sphere1;
        Polygonal*      m_sphere2;

        Panel*          m_pnl;
        VBoxLayout*     m_bloomLayout;
        HSlider*        m_radiusSlider;
        HSlider*        m_stepSlider;
        HSlider*        m_thresholdSlider;
        SceneDeferred*  dscene;

        Polygonal*      m_buddha;
};


void Application::initialize() {
        GraphicApplication::initialize();
        dscene = (SceneDeferred*) scene();

        m_room = new Polygonal("resources/models/room.xml", scene()->world());
        m_logo = new Polygonal("resources/models/logo.xml", m_room);

        m_sphere1 = new Polygonal("sphere.obj", MaterialPtr(new EmissiveMaterial()), scene()->world());
        m_sphere1->setPosition(Vec3(0,2,0));
        m_light1 = new PointLight(m_sphere1);
        m_light1->setDiffuseColor(Vec3(1.0, 1.0, 0.9));

        m_sphere2 = new Polygonal("resources/models/projector.xml", scene()->world());
        m_sphere2->setPosition(Vec3(4,2,4));
        m_sphere2->lookAt(Vec3(0), Vec3(0,1,0));
        m_light2 = new SpotLight(m_sphere2);
        m_light2->lookAt(Vec3(0), Vec3(0,1,0));

        m_buddha = new Polygonal("resources/models/buddha.xml", m_room);
        m_buddha->setPosition(Vec3(-2.0, 0.0, 2.0));

        m_diffuseBuffer = new PictureColor(dscene->gbuffer()->colorBufferTexture(0), gui()->root());
        m_materialBuffer = new PictureColor(dscene->gbuffer()->colorBufferTexture(1), gui()->root());
        m_geometryBuffer = new PictureColor(dscene->gbuffer()->colorBufferTexture(2), gui()->root());
        m_depthBuffer = new PictureDepth(dscene->gbuffer()->depthBufferTexture(), gui()->root());
        windowResizeEvent(window()->size().x, window()->size().y);

        //////////////////////////////////////////////////////

        m_pnl = new Panel("regular_panel.png", gui()->root());
        m_pnl->setOpacity(0.9);
        m_pnl->setPadding(20);
        m_pnl->setSize(Vec2(200, 300));

        m_bloomLayout = new VBoxLayout(m_pnl);

        m_radiusSlider = new HSlider("slider_background.png", "slider_selection.png", "slider_btn_active.png", "slider_btn_hover.png", m_bloomLayout);
        m_radiusSlider->setWidth(m_pnl->contentWidth());
        m_radiusSlider->setMinMaxValues(0, 200);

        m_stepSlider = new HSlider("slider_background.png", "slider_selection.png", "slider_btn_active.png", "slider_btn_hover.png", m_bloomLayout);
        m_stepSlider->setWidth(m_pnl->contentWidth());
        m_stepSlider->setMinMaxValues(1, 10);

        m_thresholdSlider = new HSlider("slider_background.png", "slider_selection.png", "slider_btn_active.png", "slider_btn_hover.png", m_bloomLayout);
        m_thresholdSlider->setWidth(m_pnl->contentWidth());

        m_bloomLayout->addWidget(new Text("Bloom radius", m_bloomLayout));
        m_bloomLayout->addWidget(m_radiusSlider);
        m_bloomLayout->addWidget(new Text("Bloom step", m_bloomLayout));
        m_bloomLayout->addWidget(m_stepSlider);
        m_bloomLayout->addWidget(new Text("Bloom threshold", m_bloomLayout));
        m_bloomLayout->addWidget(m_thresholdSlider);
        m_bloomLayout->addSpacing(20);

        HSlider* screenRadius = new  HSlider("slider_background.png", "slider_selection.png", "slider_btn_active.png", "slider_btn_hover.png", m_bloomLayout);
        screenRadius->setWidth(m_pnl->contentWidth());
        screenRadius->setMinMaxValues(1, 20);

        HSlider* viewRadius = new  HSlider("slider_background.png", "slider_selection.png", "slider_btn_active.png", "slider_btn_hover.png", m_bloomLayout);
        viewRadius->setWidth(m_pnl->contentWidth());
        viewRadius->setMinMaxValues(0.01, 4);

        HSlider* power = new  HSlider("slider_background.png", "slider_selection.png", "slider_btn_active.png", "slider_btn_hover.png", m_bloomLayout);
        power->setWidth(m_pnl->contentWidth());

        m_bloomLayout->addWidget(new Text("SSAO", m_bloomLayout));
        m_bloomLayout->addWidget(new Text("Screen radius", m_bloomLayout));
        m_bloomLayout->addWidget(screenRadius);
        m_bloomLayout->addWidget(new Text("View radius", m_bloomLayout));
        m_bloomLayout->addWidget(viewRadius);
        m_bloomLayout->addWidget(new Text("Power", m_bloomLayout));
        m_bloomLayout->addWidget(power);
        m_bloomLayout->addSpacing(20);


        dscene->postEffects()->bloom()->setRadius(0);
        auto changeBloomSettings = [&] (HSlider* slider) {
                dscene->postEffects()->bloom()->setRadius(m_radiusSlider->value());
                dscene->postEffects()->bloom()->setStep(m_stepSlider->value());
                dscene->postEffects()->bloom()->setThreshold(m_thresholdSlider->value());
        };

        m_radiusSlider->onValueChanged += changeBloomSettings;
        m_stepSlider->onValueChanged += changeBloomSettings;
        m_thresholdSlider->onValueChanged += changeBloomSettings;


        screenRadius->onValueChanged += [&] (HSlider* slider) { dscene->postEffects()->ssao()->ssaoPass()->setScreenRadius(slider->value()); };
        viewRadius->onValueChanged += [&] (HSlider* slider) { dscene->postEffects()->ssao()->ssaoPass()->setViewRadius(slider->value()); };
        power->onValueChanged += [&] (HSlider* slider) { dscene->postEffects()->ssao()->ssaoPass()->setPower(slider->value()); };

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
