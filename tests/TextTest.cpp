#include <imEngine/Application/GraphicApplication.h>
#include <imEngine/Graphics/GUI/GuiManager.h>
#include <imEngine/Graphics/GUI/Text.h>
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
        FontPtr         m_font;
        Text*           m_text1;
        Text*           m_text2;
};

void Application::initialize() {
        GraphicApplication::initialize();
        glClearColor(1,1,1,1);

        m_gui = new GuiManager("resources/gui/elementary/", mainWindow());

        m_font = FontPtr(new Font("resources/font/Lobster.ttf", 48));

        m_text1 = new Text("Text||", m_gui->root());
        m_text1->setFont(m_font);


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
        if (key == '=') m_text1->setTop(m_text1->top() + 1);
        if (key == '-') m_text1->setTop(m_text1->top() - 1);
}

int main() {
        Application app;
        return app.exec();
}
