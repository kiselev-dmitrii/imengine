#include <imEngine/Application/GraphicApplication.h>
#include <imEngine/Graphics/GUI/GuiManager.h>
#include <imEngine/Graphics/GUI/Button.h>
#include <imEngine/Graphics/GUI/Label.h>
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

private:
        GuiManager*     m_gui;
        Button*         m_btn;
        Label*          m_lbl;
};

void Application::initialize() {
        GraphicApplication::initialize();
        glClearColor(1,1,1,1);

        m_gui = new GuiManager("resources/gui/elementary/", mainWindow());

        // Создание кнопки
        m_btn = new Button("regular_btn_active.png", "regular_btn_hover.png", "regular_btn_pressed.png", "regular_btn_disabled.png", "regular_btn_focused.png");
        m_gui->attachWidget(m_btn);

        // Текст на кнопке
        m_lbl = new Label("Push it");
        m_btn->attachChild(m_lbl);
        m_lbl->setAlignment(LabelAlignment::CENTER);
        m_lbl->setWidth(m_btn->size().x);
        m_lbl->setPosition(Vec2(0, 6));

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Application::update() {
        GraphicApplication::update();
}

void Application::render() {
        GraphicApplication::render();
        m_gui->render();
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

int main() {
        Application app;
        return app.exec();
}
