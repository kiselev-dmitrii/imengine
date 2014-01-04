#include <imEngine/Application/BaseApplication.h>
#include <imEngine/Graphics/GUI/Text.h>
#include <imEngine/Utils/Debug.h>
#include "showTriangle.glsl"

using namespace imEngine;

class Application : public BaseApplication {
protected:
        void    initialize();
        void    update();
        void    render();
        void    destroy();

        void    windowResizeEvent(int x, int y) { glViewport(0,0, x, y); }
        void    keyPressEvent(int key);

private:
        Text*   m_text1;
        Text*   m_text2;
        Text*   m_fpsText;
        FontPtr m_font1;
        FontPtr m_font2;
        FontPtr m_font3;

        uint    m_fps;

        std::chrono::time_point<std::chrono::system_clock>      m_oldTime;

};

void Application::initialize() {
        glClearColor(0,0,0,0);

        m_font1 = FontPtr(new Font("resources/font/Lobster.ttf", 48));
        m_font2 = FontPtr(new Font("resources/font/FreeSans.ttf", 15));
        m_font3 = FontPtr(new Font("resources/font/FreeSans.ttf", 25));

        m_text1 = new Text("Yeah,\n bitch!", m_font1, mainWindow());
        m_text1->setColor(Vec3(0.5,0.6, 1.0));
        m_text1->setPosition(Vec2(320,240));

        m_text2 = new Text(showTriangleSource, m_font2, mainWindow());
        m_text2->setPosition(Vec2(20,20));

        m_fpsText = new Text("0 fps", m_font3, mainWindow());
        m_fpsText->setColor(Vec3(1,1,0));
        m_fpsText->setPosition(Vec2(600, 20));

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        m_fps = 0;
}

void Application::update() {
        Vec2 pos = m_text1->position();
        Keyboard* kbd = mainWindow()->keyboard();

        if (kbd->isKeyPressed(SDLK_UP)) m_text1->setPosition(pos + Vec2(0, -1));
        if (kbd->isKeyPressed(SDLK_DOWN)) m_text1->setPosition(pos + Vec2(0, 1));
        if (kbd->isKeyPressed(SDLK_LEFT)) m_text1->setPosition(pos + Vec2(-1, 0));
        if (kbd->isKeyPressed(SDLK_RIGHT)) m_text1->setPosition(pos + Vec2(1, 0));

        ++m_fps;
        auto newTime = std::chrono::system_clock::now();
        float elapsed_seconds = std::chrono::duration<double> (newTime-m_oldTime).count();
        if (elapsed_seconds > 1.0) {
                m_fpsText->setText(std::to_string(m_fps) + " fps");
                m_oldTime = newTime;
                m_fps = 0;
        }


}

void Application::render() {
        Renderer::clearBuffers();
        m_text1->render();
        m_text2->render();
        /*
        for (register int i = 0; i < 100; ++i) {
                m_text2->setColor(Vec3(float(i)/100.0, 1 - float(i)/100.0, 0));
                m_text2->render();
                m_text2->setPosition(Vec2(20, 6*i));
        }
        */
        m_fpsText->render();
}

void Application::destroy() {
        delete m_text1;
        delete m_text2;
}

void Application::keyPressEvent(int key) {
        if (key >= 32 && key < 128) {
                char ch = key;
                if (mainWindow()->keyboard()->modifiers() & KeyboardModifiers::LSHIFT) ch = toupper(ch);
                m_text1->setText(m_text1->text() + ch);
        }

}

int main() {
        Application app;
        app.exec();
}
