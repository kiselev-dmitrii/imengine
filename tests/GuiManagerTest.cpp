#include <imEngine/Application/BaseApplication.h>
#include <imEngine/Graphics/GUI/GuiManager.h>
#include <imEngine/System/Filesystem.h>
#include <imEngine/Utils/Debug.h>
#include <imEngine/Graphics/GUI/Widget.h>

using namespace imEngine;

class Application : public BaseApplication {
protected:
        void    initialize();
        void    update();
        void    render();
        void    destroy();

private:
        GuiManager*     m_gui;

};

void Application::initialize() {
        glClearColor(0,0,0,0);
        
        m_gui = new GuiManager("resources/gui/elementary/", mainWindow());
        m_gui->textureAtlas()->save("gui.png");

        for (auto str: m_gui->imageList()) {
                IM_VAR(str);
                IM_VAR(m_gui->imageGeometry(str)->size);
                IM_VAR(m_gui->imageGeometry(str)->texCoords);
        }

        new NonStretchableWidget("regular_btn_active.png", m_gui->rootWidget());
}

void Application::update() {
}

void Application::render() {
        m_gui->render();
}

void Application::destroy() {
}

int main() {
        Application app;
        app.exec();
}
