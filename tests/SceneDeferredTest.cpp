#include <imEngine/Graphics/Scene/Scene.h>
#include <imEngine/Graphics/Scene/SceneRenderer.h>
#include <imEngine/Utils/Debug.h>
#include <imEngine/Graphics/GUI/Picture.h>
#include <imEngine/Graphics/Scene/Objects/Camera/FirstPersonCamera.h>
#include <imEngine/Graphics/Scene/SceneDeferred.h>
#include <imEngine/Graphics/Scene/Objects/Light/PointLight.h>
#include <imEngine/Graphics/Scene/Objects/Light/SpotLight.h>
#include <imEngine/Graphics/Scene/Materials/EmissiveMaterial.h>

#include <imEngine/Graphics/GUI/Panel.h>
#include <imEngine/Graphics/GUI/Slider.h>
#include <imEngine/Graphics/GUI/ToggleButton.h>
#include <imEngine/Graphics/GUI/BoxLayout.h>
#include <imEngine/Graphics/Scene/Materials/RaycastingMaterial.h>
#include <imEngine/Graphics/Scene/Materials/IsosurfaceMaterial.h>

using namespace imEngine;

class Application : public GraphicApplication {
protected:
        void    initialize();
        void    keyPressEvent(int key);
        void    windowResizeEvent(int x, int y);

private:
        Entity*      m_room;
        Entity*      m_logo;
        PictureColor*   m_diffuseBuffer;
        PictureColor*   m_materialBuffer;
        PictureColor*   m_geometryBuffer;
        PictureDepth*   m_depthBuffer;

        PointLight*     m_light1;
        SpotLight*      m_light2;
        Entity*      m_sphere1;
        Entity*      m_sphere2;

        Texture3D*      m_data;
        Volume*         m_engine;

        Panel*          m_pnl;
        VBoxLayout*     m_pnlLayout;
        HSlider*        m_radiusSlider;
        HSlider*        m_stepSlider;
        HSlider*        m_thresholdSlider;

        HSlider*        alphaSlider;
        HSlider*        betaSlider;
        HSlider*        gammaSlider;
        HSlider*        distanceSlider;

        Movable*        m_empty;

        Entity*      m_buddha;

        Texture2DPtr    m_densityToColorTexture;
        PictureColor*   m_densityToColorPicture;
};


void Application::initialize() {
        GraphicApplication::initialize();

        m_room = new Entity("room.json", scene()->world());
        m_logo = new Entity("uh60.json", scene()->world());

        /*
        m_sphere1 = new Entity("sphere.obj", EntityMaterialPtr(new EmissiveMaterial()), scene()->world());
        m_sphere1->setPosition(Vec3(0,2,0));
        */
        m_light1 = new PointLight(scene()->world());
        m_light1->setPosition(Vec3(0, 10, 0));
        m_light1->setPower(2.0);
        m_light1->setDiffuseColor(Vec3(1.0, 1.0, 0.9));

        /*
        m_empty = new Movable(scene()->world());

        m_sphere2 = new Entity("projector.xml", m_empty);
        m_sphere2->setPosition(Vec3(4,2,4));
        m_sphere2->lookAt(Vec3(0), Vec3(0,1,0));
        m_light2 = new SpotLight(m_sphere2);
        m_light2->lookAt(Vec3(0), Vec3(0,1,0));
        m_light2->setDiffuseColor(Vec3(1.0, 1.0, 0.9));
        m_light2->setPower(2.0);

        m_buddha = new Entity("buddha.xml", m_room);
        m_buddha->setPosition(Vec3(-2.0, 0.0, 2.0));
        */

        m_densityToColorTexture = Texture2DPtr(new Texture2D());
        m_densityToColorTexture->load("resources/textures/density_to_color.png");
        m_data = new Texture3D();
        m_data->load(256,256,256, InternalFormat::COLOR_NORM_1_COMP_8_BIT, SourceType::UBYTE, SourceFormat::R, "resources/textures/3d/foot.raw");
        m_engine = new Volume(m_data, VolumeMaterialPtr(new RaycastingMaterial()), scene()->world());
        static_cast<RaycastingMaterial*>(m_engine->material().get())->setDensityTexture(m_densityToColorTexture);

        m_engine->setPosition(Vec3(2, 2, -2));

        /*
        m_diffuseBuffer = new PictureColor(scene()->renderer()->gBuffer()->colorBufferTexture(0), gui()->root());
        m_materialBuffer = new PictureColor(scene()->renderer()->gBuffer()->colorBufferTexture(1), gui()->root());
        m_geometryBuffer = new PictureColor(scene()->renderer()->gBuffer()->colorBufferTexture(2), gui()->root());
        m_depthBuffer = new PictureDepth(scene()->renderer()->gBuffer()->depthBufferTexture(), gui()->root());
        */
        windowResizeEvent(window()->size().x, window()->size().y);

        //////////////////////////////////////////////////////

        m_pnl = new Panel("regular_panel.png", gui()->root());
        m_pnl->setOpacity(0.9);
        m_pnl->setPadding(20);
        m_pnl->setSize(Vec2(200, 600));

        m_pnlLayout = new VBoxLayout(m_pnl);

        m_radiusSlider = new HSlider("slider_background.png", "slider_selection.png", "slider_btn_active.png", "slider_btn_hover.png", m_pnlLayout);
        m_radiusSlider->setWidth(m_pnl->contentWidth());
        m_radiusSlider->setMinMaxValues(0, 200);

        m_stepSlider = new HSlider("slider_background.png", "slider_selection.png", "slider_btn_active.png", "slider_btn_hover.png", m_pnlLayout);
        m_stepSlider->setWidth(m_pnl->contentWidth());
        m_stepSlider->setMinMaxValues(1, 10);

        m_thresholdSlider = new HSlider("slider_background.png", "slider_selection.png", "slider_btn_active.png", "slider_btn_hover.png", m_pnlLayout);
        m_thresholdSlider->setWidth(m_pnl->contentWidth());

        ToggleButton* bloomBtn = new ToggleButton("big_checkbox_active.png", "big_checkbox_checked.png", "big_checkbox_active.png", "big_checkbox_checked.png", m_pnlLayout);
        bloomBtn->setChecked(true);

        m_pnlLayout->addWidget(new Text("Bloom radius", m_pnlLayout));
        m_pnlLayout->addWidget(m_radiusSlider);
        m_pnlLayout->addWidget(new Text("Bloom step", m_pnlLayout));
        m_pnlLayout->addWidget(m_stepSlider);
        m_pnlLayout->addWidget(new Text("Bloom threshold", m_pnlLayout));
        m_pnlLayout->addWidget(m_thresholdSlider);
        m_pnlLayout->addWidget(new Text("On/off", m_pnlLayout));
        m_pnlLayout->addWidget(bloomBtn);
        m_pnlLayout->addSpacing(20);

        scene()->renderer()->bloom()->setRadius(0);
        auto changeBloomSettings = [&] (HSlider* slider) {
                scene()->renderer()->bloom()->setRadius(m_radiusSlider->value());
                scene()->renderer()->bloom()->setStep(m_stepSlider->value());
                scene()->renderer()->bloom()->setThreshold(m_thresholdSlider->value());
        };

        m_radiusSlider->onValueChanged += changeBloomSettings;
        m_stepSlider->onValueChanged += changeBloomSettings;
        m_thresholdSlider->onValueChanged += changeBloomSettings;
        bloomBtn->onClick += [&] (ToggleButton* btn) { scene()->renderer()->bloom()->setEnabled(btn->isChecked()); };

        /// SSAO

        HSlider* screenRadius = new  HSlider("slider_background.png", "slider_selection.png", "slider_btn_active.png", "slider_btn_hover.png", m_pnlLayout);
        screenRadius->setWidth(m_pnl->contentWidth());
        screenRadius->setMinMaxValues(1, 20);

        HSlider* viewRadius = new  HSlider("slider_background.png", "slider_selection.png", "slider_btn_active.png", "slider_btn_hover.png", m_pnlLayout);
        viewRadius->setWidth(m_pnl->contentWidth());
        viewRadius->setMinMaxValues(0.01, 4);

        HSlider* power = new  HSlider("slider_background.png", "slider_selection.png", "slider_btn_active.png", "slider_btn_hover.png", m_pnlLayout);
        power->setWidth(m_pnl->contentWidth());

        ToggleButton* ssaoBtn = new ToggleButton("big_checkbox_active.png", "big_checkbox_checked.png", "big_checkbox_active.png", "big_checkbox_checked.png", m_pnlLayout);
        ssaoBtn->setChecked(true);

        m_pnlLayout->addWidget(new Text("SSAO", m_pnlLayout));
        m_pnlLayout->addWidget(new Text("Screen radius", m_pnlLayout));
        m_pnlLayout->addWidget(screenRadius);
        m_pnlLayout->addWidget(new Text("View radius", m_pnlLayout));
        m_pnlLayout->addWidget(viewRadius);
        m_pnlLayout->addWidget(new Text("Power", m_pnlLayout));
        m_pnlLayout->addWidget(power);
        m_pnlLayout->addWidget(new Text("On/off", m_pnlLayout));
        m_pnlLayout->addWidget(ssaoBtn);
        m_pnlLayout->addSpacing(20);


        screenRadius->onValueChanged += [&] (HSlider* slider) { scene()->renderer()->ssao()->ssaoPass()->setScreenRadius(slider->value()); };
        viewRadius->onValueChanged += [&] (HSlider* slider) { scene()->renderer()->ssao()->ssaoPass()->setViewRadius(slider->value()); };
        power->onValueChanged += [&] (HSlider* slider) { scene()->renderer()->ssao()->ssaoPass()->setPower(slider->value()); };
        ssaoBtn->onClick += [&] (ToggleButton* btn) { scene()->renderer()->ssao()->setEnabled(btn->isChecked()); };

        /// DoF
        HSlider* maxNearRadius = new  HSlider("slider_background.png", "slider_selection.png", "slider_btn_active.png", "slider_btn_hover.png", m_pnlLayout);
        maxNearRadius->setWidth(m_pnl->contentWidth());
        maxNearRadius->setMinMaxValues(1, 200);

        HSlider* maxFarRadius = new  HSlider("slider_background.png", "slider_selection.png", "slider_btn_active.png", "slider_btn_hover.png", m_pnlLayout);
        maxFarRadius->setWidth(m_pnl->contentWidth());
        maxFarRadius->setMinMaxValues(1, 200);

        HSlider* focusStart = new  HSlider("slider_background.png", "slider_selection.png", "slider_btn_active.png", "slider_btn_hover.png", m_pnlLayout);
        focusStart->setWidth(m_pnl->contentWidth());

        HSlider* focusEnd = new  HSlider("slider_background.png", "slider_selection.png", "slider_btn_active.png", "slider_btn_hover.png", m_pnlLayout);
        focusEnd->setWidth(m_pnl->contentWidth());

        ToggleButton* dofBtn = new ToggleButton("big_checkbox_active.png", "big_checkbox_checked.png", "big_checkbox_active.png", "big_checkbox_checked.png", m_pnlLayout);
        dofBtn->setChecked(true);

        m_pnlLayout->addWidget(new Text("DoF", m_pnlLayout));
        m_pnlLayout->addWidget(new Text("Max near radius", m_pnlLayout));
        m_pnlLayout->addWidget(maxNearRadius);
        m_pnlLayout->addWidget(new Text("Max far radius", m_pnlLayout));
        m_pnlLayout->addWidget(maxFarRadius);
        m_pnlLayout->addWidget(new Text("Focus start", m_pnlLayout));
        m_pnlLayout->addWidget(focusStart);
        m_pnlLayout->addWidget(new Text("Focus end", m_pnlLayout));
        m_pnlLayout->addWidget(focusEnd);
        m_pnlLayout->addWidget(new Text("On/off", m_pnlLayout));
        m_pnlLayout->addWidget(dofBtn);
        m_pnlLayout->addSpacing(20);

        maxNearRadius->onValueChanged += [&] (HSlider* slider) { scene()->renderer()->depthOfField()->depthBlurPass()->setNearMaxRadius(slider->value()); };
        maxFarRadius->onValueChanged += [&] (HSlider* slider) { scene()->renderer()->depthOfField()->depthBlurPass()->setFarMaxRadius(slider->value()); };
        focusStart->onValueChanged += [&] (HSlider* slider) { scene()->renderer()->depthOfField()->depthBlurPass()->setFocusStart(slider->value()); };
        focusEnd->onValueChanged += [&] (HSlider* slider) { scene()->renderer()->depthOfField()->depthBlurPass()->setFocusEnd(slider->value()); };
        dofBtn->onClick += [&] (ToggleButton* btn) { scene()->renderer()->depthOfField()->setEnabled(btn->isChecked()); };

        /// Min density
        m_densityToColorPicture = new PictureColor(m_densityToColorTexture, m_pnlLayout);
        m_densityToColorPicture->setWidth(m_pnlLayout->contentWidth());
        m_pnlLayout->addWidget(m_densityToColorPicture);


        HSlider* minDensity = new  HSlider("slider_background.png", "slider_selection.png", "slider_btn_active.png", "slider_btn_hover.png", m_pnlLayout);
        minDensity->setWidth(m_pnl->contentWidth());
        m_pnlLayout->addWidget(new Text("Min density", m_pnlLayout));
        m_pnlLayout->addWidget(minDensity);

        minDensity->onValueChanged += [&] (HSlider* slider) {
                static_cast<RaycastingMaterial*>(m_engine->material().get())->setMinDensity(slider->value());
        };

        /// Clip plane
        alphaSlider = new  HSlider("slider_background.png", "slider_selection.png", "slider_btn_active.png", "slider_btn_hover.png", m_pnlLayout);
        alphaSlider->setWidth(m_pnl->contentWidth());
        alphaSlider->setMinMaxValues(0, 360);

        betaSlider = new  HSlider("slider_background.png", "slider_selection.png", "slider_btn_active.png", "slider_btn_hover.png", m_pnlLayout);
        betaSlider->setWidth(m_pnl->contentWidth());
        betaSlider->setMinMaxValues(0, 360);

        gammaSlider = new  HSlider("slider_background.png", "slider_selection.png", "slider_btn_active.png", "slider_btn_hover.png", m_pnlLayout);
        gammaSlider->setWidth(m_pnl->contentWidth());
        gammaSlider->setMinMaxValues(0, 360);

        distanceSlider = new  HSlider("slider_background.png", "slider_selection.png", "slider_btn_active.png", "slider_btn_hover.png", m_pnlLayout);
        distanceSlider->setWidth(m_pnl->contentWidth());
        distanceSlider->setMinMaxValues(-1, 1);
        distanceSlider->setValue(0);

        m_pnlLayout->addWidget(new Text("ClipPlane", m_pnlLayout));
        m_pnlLayout->addWidget(new Text("Alpha,Beta,Gamma,D", m_pnlLayout));
        m_pnlLayout->addWidget(alphaSlider);
        m_pnlLayout->addWidget(betaSlider);
        m_pnlLayout->addWidget(gammaSlider);
        m_pnlLayout->addWidget(distanceSlider);


        auto changeClipPlane = [&] (HSlider* slider) {
                Vec4 plane;
                plane.x = glm::cos(glm::radians(alphaSlider->value()));
                plane.y = glm::cos(glm::radians(betaSlider->value()));
                plane.z = glm::cos(glm::radians(gammaSlider->value()));
                plane.w = distanceSlider->value();

                static_cast<RaycastingMaterial*>(m_engine->material().get())->setClipPlane(plane);
        };
        alphaSlider->onValueChanged += changeClipPlane;
        betaSlider->onValueChanged += changeClipPlane;
        gammaSlider->onValueChanged += changeClipPlane;
        distanceSlider->onValueChanged += changeClipPlane;
}

void Application::keyPressEvent(int key) {
        GraphicApplication::keyPressEvent(key);

        if (key == SDLK_LEFT) m_empty->rotate(Vec3(0,1,0), 1.0, Space::WORLD);
        if (key == SDLK_RIGHT) m_empty->rotate(Vec3(0,1,0), -1.0, Space::WORLD);
        if (key == SDLK_UP) m_empty->rotate(Vec3(1,0,0), -1.0, Space::LOCAL);
        if (key == SDLK_DOWN) m_empty->rotate(Vec3(1,0,0), 1.0, Space::LOCAL);
        if (key == SDLK_1) m_light2->setShadowTechnique(ShadowTechniquePtr(new SimpleShadowMapping()));
        if (key == SDLK_2) m_light2->setShadowTechnique(ShadowTechniquePtr(new VarianceShadowMapping()));
        if (key == SDLK_F1) {
                bool static isEnabled = true;
                if (isEnabled) {
                        gui()->root()->hide();
                        gui()->root()->disable();
                } else {
                        gui()->root()->show();
                        gui()->root()->enable();
                }
                isEnabled = !isEnabled;
        }
}

void Application::windowResizeEvent(int x, int y) {
        GraphicApplication::windowResizeEvent(x, y);

        /*
        m_diffuseBuffer->setSize(Vec2(window()->size())/8.0f);
        m_diffuseBuffer->setPosition(Vec2(0,0));
        m_materialBuffer->setSize(Vec2(window()->size())/8.0f);
        m_materialBuffer->setPosition(Vec2(m_diffuseBuffer->width(),0));
        m_geometryBuffer->setSize(Vec2(window()->size())/8.0f);
        m_geometryBuffer->setPosition(Vec2(2*m_diffuseBuffer->width(),0));
        m_depthBuffer->setSize(Vec2(window()->size())/8.0f);
        m_depthBuffer->setPosition(Vec2(3*m_diffuseBuffer->width(),0));
        */
}

int main() {
        Application app;
        return app.exec();
}
