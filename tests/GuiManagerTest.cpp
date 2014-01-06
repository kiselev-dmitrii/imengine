#include <imEngine/Application/BaseApplication.h>
#include <imEngine/Graphics/GUI/GuiManager.h>
#include <imEngine/System/Filesystem.h>
#include <imEngine/Utils/Debug.h>

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
        
        m_gui = new GuiManager("resources/gui/elementary");
        m_gui->textureAtlas()->save("gui.png");

        for (auto str: Filesystem::tree(m_gui->themePath())) {
                IM_VAR(str);
                IM_VAR(m_gui->imageGeometry(str)->size);
                IM_VAR(m_gui->imageGeometry(str)->texCoords);
        }
}

void Application::update() {
}

void Application::render() {
}

void Application::destroy() {
}

int main() {
        Application app;
        app.exec();
}
