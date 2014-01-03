#include <imEngine/Application/BaseApplication.h>
#include <imEngine/Graphics/GUI/Text.h>
#include <imEngine/Utils/Debug.h>

using namespace imEngine;

class Application : public BaseApplication {
protected:
        void    initialize();
        void    update();
        void    render();
        void    destroy();

        void    windowResizeEvent(int x, int y) { glViewport(0,0, x, y); }

private:
        Text*   m_text1;
        Text*   m_text2;
        FontPtr m_font1;
        FontPtr m_font2;
};

void Application::initialize() {
        glClearColor(0,0,0,1);

        m_font1 = FontPtr(new Font("resources/font/Lobster.ttf", 48));
        m_font2 = FontPtr(new Font("resources/font/FreeSans.ttf", 15));

        m_text1 = new Text("Yeah, bitch!", m_font1, mainWindow());
        m_text1->setColor(Vec3(0.5,0.6, 1.0));
        m_text1->setPosition(Vec2(320,240));

        m_text2 = new Text("This is imEngine", m_font2, mainWindow());
        m_text2->setPosition(Vec2(20,20));


        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Application::update() {
}

void Application::render() {
        Renderer::clearBuffers();
        m_text1->render();
        m_text2->render();
}

void Application::destroy() {
        delete m_text1;
}

int main() {
        Application app;
        app.exec();
}
