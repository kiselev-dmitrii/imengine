#include <imEngine/Application/GraphicApplication.h>
#include <imEngine/FileContainers/Mesh.h>
#include <imEngine/Graphics/Scene/Geometry.h>
#include <imEngine/Utils/Debug.h>

using namespace imEngine;

class Application : public GraphicApplication {
protected:
        void    initialize();
        void    update();
        void    render();
        void    destroy();

private:
        Geometry*       m_geo;
};

void Application::initialize() {
        GraphicApplication::initialize();
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        Mesh mesh("resources/models/cube.obj");
        m_geo = new Geometry(mesh);
}

void Application::update() {
        GraphicApplication::update();
}

void Application::render() {
        GraphicApplication::render();
}

void Application::destroy() {
        GraphicApplication::destroy();
}

int main() {
        Application app;
        return app.exec();
}
