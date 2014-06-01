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
#include <imEngine/Graphics/GUI/LineEdit.h>
#include <imEngine/Graphics/GUI/TextButton.h>
#include <imEngine/Graphics/Scene/Materials/RaycastingMaterial.h>
#include <imEngine/Graphics/Scene/Materials/IsosurfaceMaterial.h>
#include <imEngine/System/Filesystem.h>

#include <sstream>
#include <iomanip>
#include <boost/filesystem.hpp>

using namespace imEngine;

String toString(float value) {
        std::stringstream ss;
        ss << std::setprecision(4) << value;
        return ss.str();
}

class TextSlider : public HBoxLayout {
public:
        TextSlider(float min, float max, float width, WidgetAbstract* parent);

public:
        HSlider*        slider;
        Text*           value;
};

TextSlider::TextSlider(float min, float max, float width, WidgetAbstract *parent) :
        HBoxLayout(parent)
{
        setWidth(width);

        slider = new HSlider("slider_background.png", "slider_selection.png", "slider_btn_active.png", "slider_btn_hover.png", this);
        slider->setWidth(width - 40);
        slider->setMinMaxValues(min, max);
        addWidget(slider);

        value = new Text("0", this);
        addWidget(value);

        slider->onValueChanged += [&] (HSlider* slider) {
               value->setText(toString(slider->value()));
        };
}

class Application : public GraphicApplication {
protected:
        void    initialize();
        void    keyPressEvent(int key);
        void    windowResizeEvent(int x, int y);

private:
        Entity*      m_room;
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
        LineEdit*     lut_path;
        LineEdit*     data_path;

        Panel*          m_pnl;
        VBoxLayout*     m_pnlLayout;
        HSlider*        m_radiusSlider;
        HSlider*        m_stepSlider;
        HSlider*        m_thresholdSlider;

        TextSlider*     alpha;
        TextSlider*     beta;
        TextSlider*     gamma;
        TextSlider*     shift;

        Movable*        m_empty;

        Entity*      m_buddha;

        Texture2DPtr    m_densityToColorTexture;
        PictureColor*   m_densityToColorPicture;

        RaycastingMaterial*  raycastingMaterial;
        IsosurfaceMaterial*  isosurfaceMaterial;

};


void Application::initialize() {
        GraphicApplication::initialize();
        scene()->renderer()->bloom()->setEnabled(false);
        scene()->renderer()->ssao()->setEnabled(false);
        scene()->renderer()->depthOfField()->setEnabled(false);

        m_room = new Entity("sponza.json", scene()->world());

        Entity* vase1 = new Entity("vase.json", m_room);
        Entity* vase2 = new Entity("vase.json", m_room);
        vase2->setPosition(Vec3(1, 0.0, 1));
        Entity* vase3 = new Entity("vase.json", m_room);
        vase3->setPosition(Vec3(2, 0.0, 2));
        Entity* vase4 = new Entity("vase.json", m_room);
        vase4->setPosition(Vec3(3, 0.0, 3));
        Entity* vase5 = new Entity("vase.json", m_room);
        vase5->setPosition(Vec3(4, 0.0, 4));

        m_sphere1 = new Entity("light_sphere.json", scene()->world());
        m_light1 = new PointLight(m_sphere1);
        m_light1->setDiffuseColor(Vec3(1.0, 1.0, 0.9));
        m_sphere1->setPosition(Vec3(0.0, 2.0, 1.0));
        m_light1->setPower(1.0);
        m_light1->setAttenuation(0.0);

        m_empty = new Movable(scene()->world());

        m_sphere2 = new Entity("projector.json", m_empty);
        m_sphere2->setPosition(Vec3(4,2,4));
        m_sphere2->lookAt(Vec3(0), Vec3(0,1,0));
        m_light2 = new SpotLight(m_sphere2);
        m_light2->lookAt(Vec3(0), Vec3(0,1,0));
        m_light2->setDiffuseColor(Vec3(1.0, 1.0, 0.9));
        m_light2->setPower(80.0);


        /*
        m_buddha = new Entity("buddha.xml", m_room);
        m_buddha->setPosition(Vec3(-2.0, 0.0, 2.0));
        */

        m_densityToColorTexture = Texture2DPtr(new Texture2D());
        m_densityToColorTexture->load("resources/textures/density_to_color.png");
        m_data = new Texture3D();
        m_data->load(256,256, 256, InternalFormat::COLOR_NORM_1_COMP_8_BIT, SourceType::UBYTE, SourceFormat::R, "resources/textures/3d/foot.raw");
        m_engine = new Volume(m_data, VolumeMaterialPtr(new RaycastingMaterial()), scene()->world());
        m_engine->material()->setDensityTexture(m_densityToColorTexture);

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
        m_pnl->setSize(Vec2(300, 360));

        m_pnlLayout = new VBoxLayout(m_pnl);

        /*
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
        */
        /// Data
        m_pnlLayout->addWidget(new Text("Data:", m_pnlLayout));
        HBoxLayout* data_load = new HBoxLayout(m_pnlLayout);
        data_load->setWidth(m_pnl->contentWidth());
        m_pnlLayout->addWidget(data_load);

        data_path = new LineEdit("regular_lineedit_active.png", "regular_lineedit_disabled.png", "regular_lineedit_focused.png", data_load);
        data_path->setPadding(9);
        data_path->setWidth(data_load->width()*0.75);
        data_load->addWidget(data_path);

        TextButton* data_btn = new TextButton("Load", "regular_btn_active.png", "regular_btn_hover.png", "regular_btn_pressed.png", "regular_btn_disabled.png", "regular_btn_focused.png", data_load);
        data_btn->setWidth(data_load->width()*0.25);
        data_load->addWidget(data_btn);

        data_btn->onClick += [&] (Button* button) {
                String path = Filesystem::joinPath("resources/textures/3d", data_path->text());
                IM_VAR(path);
                if (boost::filesystem::exists(path)) {
                        IM_LOG("Found");
                        m_data->load(256,256,256, InternalFormat::COLOR_NORM_1_COMP_8_BIT, SourceType::UBYTE, SourceFormat::R, path);
                        m_engine->setData(m_data);
                }
        };


        /// Look up table
        m_pnlLayout->addWidget(new Text("Look up table:", m_pnlLayout));
        HBoxLayout* lut_load = new HBoxLayout(m_pnlLayout);
        lut_load->setWidth(m_pnl->contentWidth());
        m_pnlLayout->addWidget(lut_load);

        lut_path = new LineEdit("regular_lineedit_active.png", "regular_lineedit_disabled.png", "regular_lineedit_focused.png", lut_load);
        lut_path->setPadding(9);
        lut_path->setWidth(lut_load->width()*0.75);
        lut_load->addWidget(lut_path);

        TextButton* lut_btn = new TextButton("Load", "regular_btn_active.png", "regular_btn_hover.png", "regular_btn_pressed.png", "regular_btn_disabled.png", "regular_btn_focused.png", lut_load);
        lut_btn->setWidth(lut_load->width()*0.25);
        lut_load->addWidget(lut_btn);

        lut_btn->onClick += [&] (Button* button) {
                String path = Filesystem::joinPath("resources/textures", lut_path->text());
                IM_VAR(path);
                if (boost::filesystem::exists(path)) {
                        IM_LOG("Found");
                        m_densityToColorTexture->load(path);
                }
        };

        m_densityToColorPicture = new PictureColor(m_densityToColorTexture, m_pnlLayout);
        m_densityToColorPicture->setWidth(m_pnl->contentWidth());
        m_densityToColorPicture->setHeight(30);

        m_pnlLayout->addWidget(m_densityToColorPicture);
        m_pnlLayout->addSpacing(10);

        /// Step
        TextSlider* step = new TextSlider(1, 10, m_pnl->contentWidth(), m_pnlLayout);
        m_pnlLayout->addWidget(new Text("Step:", m_pnlLayout));
        m_pnlLayout->addWidget(step);
        step->slider->onValueChanged += [&] (HSlider* slider) {
                m_engine->material()->setStep(slider->value());
        };

        /// Raycasting Material
        raycastingMaterial = dynamic_cast<RaycastingMaterial*>(m_engine->material().get());
        isosurfaceMaterial = dynamic_cast<IsosurfaceMaterial*>(m_engine->material().get());
        if (raycastingMaterial) {
                /// MinDensity
                TextSlider* minDensity = new TextSlider(0, 1, m_pnl->contentWidth(), m_pnlLayout);
                m_pnlLayout->addWidget(new Text("Min Density:", m_pnlLayout));
                m_pnlLayout->addWidget(minDensity);
                minDensity->slider->onValueChanged += [&] (HSlider* slider) {
                        raycastingMaterial->setMinDensity(slider->value());
                };

                /// MaxDensity
                TextSlider* maxDensity = new TextSlider(0, 1, m_pnl->contentWidth(), m_pnlLayout);
                m_pnlLayout->addWidget(new Text("Max Density:", m_pnlLayout));
                m_pnlLayout->addWidget(maxDensity);
                maxDensity->slider->onValueChanged += [&] (HSlider* slider) {
                        raycastingMaterial->setMaxDensity(slider->value());
                };

                /// ClipPlane
                alpha = new TextSlider(0, 360, m_pnl->contentWidth(), m_pnlLayout);
                beta = new TextSlider(0, 360, m_pnl->contentWidth(), m_pnlLayout);
                gamma = new TextSlider(0, 360, m_pnl->contentWidth(), m_pnlLayout);
                shift = new TextSlider(0, 10, m_pnl->contentWidth(), m_pnlLayout);

                m_pnlLayout->addWidget(new Text("Clip Plane:", m_pnlLayout));
                m_pnlLayout->addWidget(new Text("alpha:", m_pnlLayout));
                m_pnlLayout->addWidget(alpha);
                m_pnlLayout->addWidget(new Text("beta:", m_pnlLayout));
                m_pnlLayout->addWidget(beta);
                m_pnlLayout->addWidget(new Text("gamma:", m_pnlLayout));
                m_pnlLayout->addWidget(gamma);
                m_pnlLayout->addWidget(new Text("shift:", m_pnlLayout));
                m_pnlLayout->addWidget(shift);
                auto changeClipPlane = [&] (HSlider* slider) {
                        Vec4 plane;
                        plane.x = glm::cos(glm::radians(alpha->slider->value()));
                        plane.y = glm::cos(glm::radians(beta->slider->value()));
                        plane.z = glm::cos(glm::radians(gamma->slider->value()));
                        plane.w = shift->slider->value();
                        IM_VAR(plane);
                        raycastingMaterial->setClipPlane(plane);
                };
                alpha->slider->onValueChanged += changeClipPlane;
                beta->slider->onValueChanged += changeClipPlane;
                gamma->slider->onValueChanged += changeClipPlane;
                shift->slider->onValueChanged += changeClipPlane;

        } else {
                /// MaxDensity
                TextSlider* threshold = new TextSlider(0, 1, m_pnl->contentWidth(), m_pnlLayout);
                m_pnlLayout->addWidget(new Text("Threshold:", m_pnlLayout));
                m_pnlLayout->addWidget(threshold);
                threshold->slider->onValueChanged += [&] (HSlider* slider) {
                        isosurfaceMaterial->setThresholdDensity(slider->value());
                };
        }
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
