#include <imEngine/Application/GraphicApplication.h>
#include <imEngine/Graphics/GUI/GuiManager.h>
#include <imEngine/Graphics/GUI/Button.h>
#include <imEngine/Graphics/GUI/ToggleButton.h>
#include <imEngine/Graphics/GUI/Panel.h>
#include <imEngine/Graphics/GUI/BoxLayout.h>
#include <imEngine/Graphics/GUI/Slider.h>
#include <imEngine/Graphics/GUI/Picture.h>
#include <imEngine/Utils/Debug.h>

using namespace imEngine;

class Application : public GraphicApplication {
protected:
        void    initialize();
        void    update();
        void    render();
        void    destroy();

        void    mouseMoveEvent(int oldX, int oldY, int newX, int newY);
        void    mousePressEvent(int x, int y, char button);
        void    mouseReleaseEvent(int x, int y, char button);
        void    keyPressEvent(int key);

private:
        GuiManager*     m_gui;
        Panel*          m_pnl;
        Button*         m_btnPlus;
        Button*         m_btnMinus;
        HSlider*        m_slider;
        HBoxLayout*     m_lt;

        Button*         m_btn1;
        Button*         m_btn2;
        Button*         m_btn3;
        ToggleButton*   m_toggle;
        VBoxLayout*     m_vlt;

        Picture*        m_pic;
};

void Application::initialize() {
        GraphicApplication::initialize();
        glClearColor(1,1,1,1);

        m_gui = new GuiManager("resources/gui/elementary/", mainWindow());

        // Создаем панельку
        m_pnl = new Panel("regular_panel.png", m_gui->root());
        m_pnl->setSize(Vec2(700, 500));

        // Создаем HBoxLayout
        m_lt = new HBoxLayout(m_pnl);
        m_lt->setPosition(Vec2(100, 200));

        // Создание кнопок и слайдера
        m_btnMinus = new Button("regular_btn_active.png", "regular_btn_hover.png", "regular_btn_pressed.png", "regular_btn_disabled.png", "regular_btn_focused.png", m_lt);
        m_lt->addWidget(m_btnMinus, WidgetVAlignment::CENTER);

        m_slider = new HSlider("slider_background.png", "slider_selection.png", "slider_btn_active.png", "slider_btn_hover.png", m_lt);
        m_slider->setWidth(300);
        m_lt->addWidget(m_slider, WidgetVAlignment::CENTER);

        m_btnPlus = new Button("regular_btn_active.png", "regular_btn_hover.png", "regular_btn_pressed.png", "regular_btn_disabled.png", "regular_btn_focused.png", m_lt);
        m_lt->addWidget(m_btnPlus, WidgetVAlignment::CENTER);


        // Создаем VBoxLayout и цепляем к нему 3 кнопки
        m_vlt = new VBoxLayout(m_lt);
        m_vlt->setPosition(Vec2(30,30));

        m_btn1 = new Button("regular_btn_active.png", "regular_btn_hover.png", "regular_btn_pressed.png", "regular_btn_disabled.png", "regular_btn_focused.png", m_vlt);
        m_vlt->addWidget(m_btn1);

        m_btn2 = new Button("regular_btn_active.png", "regular_btn_hover.png", "regular_btn_pressed.png", "regular_btn_disabled.png", "regular_btn_focused.png", m_vlt);
        m_vlt->addWidget(m_btn2);

        m_btn3 = new Button("regular_btn_active.png", "regular_btn_hover.png", "regular_btn_pressed.png", "regular_btn_disabled.png", "regular_btn_focused.png", m_vlt);
        m_vlt->addWidget(m_btn3);

        m_toggle = new ToggleButton("big_checkbox_active.png", "big_checkbox_checked.png", "big_checkbox_active.png", "big_checkbox_checked.png", m_vlt);
        m_vlt->addWidget(m_toggle, WidgetHAlignment::CENTER);

        m_lt->insertWidget(m_vlt, 0);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Application::update() {
        GraphicApplication::update();
}

void Application::render() {
        GraphicApplication::render();
        m_gui->processRender();
}

void Application::destroy() {
        delete m_gui;
}

void Application::mouseMoveEvent(int oldX, int oldY, int newX, int newY) {
        m_gui->processMouseMove(oldX, oldY, newX, newY);
}

void Application::mousePressEvent(int x, int y, char button) {
        m_gui->processMousePress(x, y, button);
}

void Application::mouseReleaseEvent(int x, int y, char button) {
        m_gui->processMouseRelease(x, y, button);
}

void Application::keyPressEvent(int key) {
        if (key == '-') m_slider->setWidth(m_slider->width()-1);
        if (key == '=') m_slider->setWidth(m_slider->width()+1);
        if (key == '1') m_slider->setVisible(!m_slider->isVisible());
}

int main() {
        Application app;
        return app.exec();
}
