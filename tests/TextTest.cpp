#include <imEngine/Application/BaseApplication.h>
#include <imEngine/Graphics/GUI/Text.h>

using namespace imEngine;

class Application : public BaseApplication {
protected:
        void    initialize();
        void    update();
        void    render();
        void    destroy();

private:
        Text*   m_text1;
        Text*   m_text2;
};

void Application::initialize() {
        glClearColor(0,0,0,1);

        m_text1 = new Text("Hello, world!", FontPtr(new Font("resources/font/Lobster.ttf", 48)));
        m_text1->setWindow(mainWindow());
        m_text1->setPosition(Vec2(320,240));

        m_text2 = new Text("imEngine", FontPtr(new Font("resources/font/FreeSans.ttf", 15)));
        m_text2->setWindow(mainWindow());
        m_text2->setPosition(Vec2(20,20));

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Application::update() {
}

void Application::render() {
        Renderer::clearBuffers();
        m_text2->render();
        m_text1->render();
        m_text2->render();
        m_text1->render();
}

void Application::destroy() {
}

int main() {
        Application app;
        app.exec();
}
