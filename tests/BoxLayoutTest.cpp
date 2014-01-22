#include <imEngine/Application/GraphicApplication.h>
#include <imEngine/Graphics/GUI/GuiManager.h>
#include <imEngine/Graphics/GUI/Button.h>
#include <imEngine/Graphics/GUI/Panel.h>
#include <imEngine/Graphics/GUI/BoxLayout.h>
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
        Button*         m_btn1;
        Button*         m_btn2;
        Button*         m_btn3;
        Button*         m_btn4;
        HBoxLayout*     m_lt;
};

void Application::initialize() {
        GraphicApplication::initialize();
        glClearColor(1,1,1,1);

        m_gui = new GuiManager("resources/gui/elementary/", mainWindow());

        // Создаем панельку
        m_pnl = new Panel("regular_panel.png", m_gui->root());
        m_pnl->setSize(Vec2(400, 500));

        // Создаем HBoxLayout
        m_lt = new HBoxLayout(m_pnl);
        m_lt->setPosition(Vec2(100, 200));

        // Создание кнопок и добавление в m_lt
        m_btn1 = new Button("regular_btn_active.png", "regular_btn_hover.png", "regular_btn_pressed.png", "regular_btn_disabled.png", "regular_btn_focused.png", m_lt);
        m_lt->addWidget(m_btn1);

        m_lt->addStretch(0.5);

        m_btn2 = new Button("regular_btn_active.png", "regular_btn_hover.png", "regular_btn_pressed.png", "regular_btn_disabled.png", "regular_btn_focused.png", m_lt);
        m_lt->addWidget(m_btn2);

        m_lt->addStretch(1);

        m_btn3 = new Button("regular_btn_active.png", "regular_btn_hover.png", "regular_btn_pressed.png", "regular_btn_disabled.png", "regular_btn_focused.png", m_lt);
        m_lt->addWidget(m_btn3);

        m_lt->addSpacing(30);

        m_btn4 = new Button("regular_btn_active.png", "regular_btn_hover.png", "regular_btn_pressed.png", "regular_btn_disabled.png", "regular_btn_focused.png", m_lt);
        m_lt->addWidget(m_btn4);

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
        if (key == '-') m_lt->setWidth(m_lt->width()-1);
        if (key == '=') m_lt->setWidth(m_lt->width()+1);
        if (key == '1') m_btn2->setVisible(!m_btn2->isVisible());
}

int main() {
        Application app;
        return app.exec();
}
