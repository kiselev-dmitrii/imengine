#include <imEngine/Application/GraphicApplication.h>
#include <imEngine/Graphics/GUI/GuiManager.h>
#include <imEngine/Graphics/GUI/Button.h>
#include <imEngine/Graphics/GUI/Label.h>
#include <imEngine/Graphics/GUI/Panel.h>
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
        Panel*          m_pnl;
        Label*          m_lblPanelName;
};

void Application::initialize() {
        GraphicApplication::initialize();
        glClearColor(1,1,1,1);

        m_gui = new GuiManager("resources/gui/elementary/", mainWindow());

        m_pnl = new Panel("regular_panel.png", m_gui->root());
        m_pnl->setSize(Vec2(400, 500));

        // Создание кнопки
        m_btn = new Button("regular_btn_active.png", "regular_btn_hover.png", "regular_btn_pressed.png", "regular_btn_disabled.png", "regular_btn_focused.png", m_pnl);

        // Текст на кнопке
        m_lbl = new Label("0", m_btn);
        m_lbl->setAlignment(LabelAlignment::CENTER);
        m_lbl->setWidth(m_btn->size().x);
        m_lbl->setPosition(Vec2(0, 6));

        m_lblPanelName = new Label("My own panel", m_pnl);
        m_lblPanelName->setFont(FontPtr(new Font("resources/font/Lobster.ttf", 23)));
        m_lblPanelName->setAlignment(LabelAlignment::CENTER);
        m_lblPanelName->setWidth(m_pnl->size().x);
        m_lblPanelName->setPosition(Vec2(0, 20));


        m_btn->onClick += [&] (Button* sender) { m_lbl->setText(std::to_string(std::stoi(m_lbl->text())+ 1)); };
        m_btn->onClick += [&] (Button* sender) { sender->setPosition(sender->position() + Vec2(4,4)); };

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
