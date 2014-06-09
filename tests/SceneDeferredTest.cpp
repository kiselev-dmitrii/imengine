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
        void    windowResizeEvent(int x, int y);
        void    keyPressEvent(int key);

private:
        Entity*      m_room;

        PointLight*     m_light1;
        SpotLight*      m_light2;
        Entity*         m_sphere1;
        Entity*         m_sphere2;

        Texture3D*      m_data;
        Volume*         m_engine;
        Movable*        m_empty;

        PictureColor*   m_diffuseBuffer;
        PictureColor*   m_materialBuffer;
        PictureColor*   m_geometryBuffer;
        PictureDepth*   m_depthBuffer;
};


void Application::initialize() {
        GraphicApplication::initialize();

        /*
        m_room = new Entity("room.json", scene()->world());

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
        m_light1->setDiffuseColor(Vec3(0.3, 1.0, 0.9));
        m_sphere1->setPosition(Vec3(0.0, 2.0, 1.0));
        m_light1->setPower(10.0);
        m_light1->setAttenuation(0.5);

        m_sphere1 = new Entity("light_sphere.json", scene()->world());
        m_light1 = new PointLight(m_sphere1);
        m_light1->setDiffuseColor(Vec3(1.0, 1.0, 0.3));
        m_sphere1->setPosition(Vec3(0.0, 2.0, 1.0));
        m_light1->setPower(10.0);
        m_light1->setAttenuation(0.5);


        m_sphere2 = new Entity("projector.json", scene()->world());
        m_sphere2->setPosition(Vec3(4,2,4));
        m_sphere2->lookAt(Vec3(0), Vec3(0,1,0));
        m_light2 = new SpotLight(m_sphere2);
        m_light2->lookAt(Vec3(0), Vec3(0,1,0));
        m_light2->setDiffuseColor(Vec3(1.0, 1.0, 0.9));
        m_light2->setPower(2.0);
        m_light2->setCutoffAngle(45);
        m_light2->setAttenuation(0.001);

        m_sphere2 = new Entity("projector.json", scene()->world());
        m_sphere2->setPosition(Vec3(4,2,4));
        m_sphere2->lookAt(Vec3(0), Vec3(0,1,0));
        m_light2 = new SpotLight(m_sphere2);
        m_light2->lookAt(Vec3(0), Vec3(0,1,0));
        m_light2->setDiffuseColor(Vec3(1.0, 1.0, 0.9));
        m_light2->setPower(2.0);
        m_light2->setCutoffAngle(45);
        m_light2->setAttenuation(0.001);


        Texture2DPtr densityToColorTexture = Texture2DPtr(new Texture2D());
        densityToColorTexture->load("resources/textures/density_to_color.png");
        m_data = new Texture3D();
        m_data->load(256,256,256, InternalFormat::COLOR_NORM_1_COMP_8_BIT, SourceType::UBYTE, SourceFormat::R, "resources/textures/3d/foot.raw");
        m_engine = new Volume(m_data, VolumeMaterialPtr(new RaycastingMaterial()), scene()->world());
        static_cast<RaycastingMaterial*>(m_engine->material().get())->setDensityTexture(densityToColorTexture);

        m_engine->setPosition(Vec3(2, -2, -2));
        */

        scene()->loadScene("room.json");
        gui()->loadLayout("deferred_test_layout.xml");

        /*
        m_diffuseBuffer = new PictureColor(scene()->renderer()->gBuffer()->colorBufferTexture(0), gui()->root());
        m_materialBuffer = new PictureColor(scene()->renderer()->gBuffer()->colorBufferTexture(1), gui()->root());
        m_geometryBuffer = new PictureColor(scene()->renderer()->gBuffer()->colorBufferTexture(2), gui()->root());
        m_depthBuffer = new PictureDepth(scene()->renderer()->gBuffer()->depthBufferTexture(), gui()->root());
        */
        windowResizeEvent(window()->size().x, window()->size().y);

        gui()->root()->dbgShowHierarchy();

        //////////////////////////////////////////////////////
        HSlider* bloomRadius = (HSlider*) gui()->root()->find("bloom_radius", true);
        HSlider* bloomStep = (HSlider*) gui()->root()->find("bloom_step", true);
        HSlider* bloomThreshold = (HSlider*) gui()->root()->find("bloom_threshold", true);
        ToggleButton* bloomEnabled = (ToggleButton*) gui()->root()->find("bloom_enabled", true);

        HSlider* ssaoSRadius = (HSlider*) gui()->root()->find("ssao_sradius", true);
        HSlider* ssaoVRadius = (HSlider*) gui()->root()->find("ssao_vradius", true);
        HSlider* ssaoPower = (HSlider*) gui()->root()->find("ssao_power", true);
        ToggleButton* ssaoEnabled = (ToggleButton*) gui()->root()->find("ssao_enabled", true);

        HSlider* dofNear = (HSlider*) gui()->root()->find("dof_near", true);
        HSlider* dofFar = (HSlider*) gui()->root()->find("dof_far", true);
        HSlider* dofStart = (HSlider*) gui()->root()->find("dof_start", true);
        HSlider* dofEnd = (HSlider*) gui()->root()->find("dof_end", true);
        ToggleButton* dofEnabled = (ToggleButton*) gui()->root()->find("dof_enabled", true);

        HSlider* ssrrStep = (HSlider*) gui()->root()->find("ssrr_step", true);
        HSlider* ssrrMaxSamples = (HSlider*) gui()->root()->find("ssrr_max_samples", true);
        HSlider* ssrrNumRefinements = (HSlider*) gui()->root()->find("ssrr_num_refinements", true);
        ToggleButton* ssrrEnabled = (ToggleButton*) gui()->root()->find("ssrr_enabled", true);

        Button* btnLogo = (Button*) gui()->root()->find("btn_logo", true);

        bloomRadius->onValueChanged += [&] (HSlider* slider) {
                scene()->renderer()->bloom()->setRadius(slider->value());
                ((Text*)slider->parent()->find("bloom_radius_value", false))->setText(std::to_string(slider->value()));
        };
        bloomStep->onValueChanged += [&] (HSlider* slider) {
                scene()->renderer()->bloom()->setStep(slider->value());
                ((Text*)slider->parent()->find("bloom_step_value", false))->setText(std::to_string(slider->value()));
        };
        bloomThreshold->onValueChanged += [&] (HSlider* slider) {
                scene()->renderer()->bloom()->setThreshold(slider->value());
                ((Text*)slider->parent()->find("bloom_threshold_value", false))->setText(std::to_string(slider->value()));
        };
        bloomEnabled->onClick += [&] (ToggleButton* button) { scene()->renderer()->bloom()->setEnabled(button->isChecked()); };

        ssaoSRadius->onValueChanged += [&] (HSlider* slider) {
                scene()->renderer()->ssao()->ssaoPass()->setScreenRadius(slider->value());
                ((Text*)slider->parent()->find("ssao_sradius_value", false))->setText(std::to_string(slider->value()));
        };
        ssaoVRadius->onValueChanged += [&] (HSlider* slider) {
                scene()->renderer()->ssao()->ssaoPass()->setViewRadius(slider->value());
                ((Text*)slider->parent()->find("ssao_vradius_value", false))->setText(std::to_string(slider->value()));
        };
        ssaoPower->onValueChanged += [&] (HSlider* slider) {
                scene()->renderer()->ssao()->ssaoPass()->setPower(slider->value());
                ((Text*)slider->parent()->find("ssao_power_value", false))->setText(std::to_string(slider->value()));
        };
        ssaoEnabled->onClick += [&] (ToggleButton* button) { scene()->renderer()->ssao()->setEnabled(button->isChecked()); };

        dofNear->onValueChanged += [&] (HSlider* slider) {
                scene()->renderer()->depthOfField()->depthBlurPass()->setNearMaxRadius(slider->value());
                ((Text*)slider->parent()->find("dof_near_value", false))->setText(std::to_string(slider->value()));
        };
        dofFar->onValueChanged += [&] (HSlider* slider) {
                scene()->renderer()->depthOfField()->depthBlurPass()->setFarMaxRadius(slider->value());
                ((Text*)slider->parent()->find("dof_far_value", false))->setText(std::to_string(slider->value()));
        };
        dofStart->onValueChanged += [&] (HSlider* slider) {
                scene()->renderer()->depthOfField()->depthBlurPass()->setFocusStart(slider->value());
                ((Text*)slider->parent()->find("dof_start_value", false))->setText(std::to_string(slider->value()));
        };
        dofEnd->onValueChanged += [&] (HSlider* slider) {
                scene()->renderer()->depthOfField()->depthBlurPass()->setFocusEnd(slider->value());
                ((Text*)slider->parent()->find("dof_end_value", false))->setText(std::to_string(slider->value()));
        };
        dofEnabled->onClick += [&] (ToggleButton* button) { scene()->renderer()->depthOfField()->setEnabled(button->isChecked()); };

        ssrrStep->onValueChanged += [&] (HSlider* slider) {
                scene()->renderer()->reflections()->reflectionPass()->setViewSpaceStep(slider->value());
                ((Text*)slider->parent()->find("ssrr_step_value", false))->setText(std::to_string(slider->value()));
        };
        ssrrMaxSamples->onValueChanged += [&] (HSlider* slider) {
                scene()->renderer()->reflections()->reflectionPass()->setMaxNumSamples(slider->value());
                ((Text*)slider->parent()->find("ssrr_max_samples_value", false))->setText(std::to_string(slider->value()));
        };
        ssrrNumRefinements->onValueChanged += [&] (HSlider* slider) {
                scene()->renderer()->reflections()->reflectionPass()->setNumRefinemetns(slider->value());
                ((Text*)slider->parent()->find("ssrr_num_refinements_value", false))->setText(std::to_string(slider->value()));
        };
        ssrrEnabled->onClick += [&] (ToggleButton* button) { scene()->renderer()->reflections()->setEnabled(button->isChecked()); };



        btnLogo->onClick += [&] (Button* button) {
                Panel* settings = (Panel*) gui()->root()->find("settings", true);
                if (settings->isVisible()) settings->hide();
                else settings->show();
        };
}

void Application::windowResizeEvent(int x, int y) {
        GraphicApplication::windowResizeEvent(x, y);

        /*
        m_diffuseBuffer->setSize(Vec2(window()->size())/2.0f);
        m_diffuseBuffer->setPosition(Vec2(0,0));
        m_materialBuffer->setSize(Vec2(window()->size())/2.0f);
        m_materialBuffer->setPosition(Vec2(m_diffuseBuffer->width(),0));
        m_geometryBuffer->setSize(Vec2(window()->size())/2.0f);
        m_geometryBuffer->setPosition(Vec2(0,m_diffuseBuffer->height()));
        m_depthBuffer->setSize(Vec2(window()->size())/2.0f);
        m_depthBuffer->setPosition(Vec2(m_diffuseBuffer->width(),m_diffuseBuffer->height()));
        */
}

void Application::keyPressEvent(int key) {
        if (key == SDLK_F1) scene()->saveScene("my_scene.json");
}

int main() {
        Application app;
        return app.exec();
}
