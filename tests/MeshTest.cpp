#include <imEngine/Application/GraphicApplication.h>
#include <imEngine/FileContainers/Mesh.h>
#include <imEngine/Utils/Debug.h>

using namespace imEngine;

class Application : public GraphicApplication {
protected:
        void    initialize();
        void    update();
        void    render();
        void    destroy();
};

void Application::initialize() {
        GraphicApplication::initialize();
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        Mesh mesh("resources/models/cube.obj");
        IM_VAR(mesh.vertices().size());
        for (const Vertex& v: mesh.vertices()) {
                IM_VAR(v.position);
                IM_VAR(v.normal);
                IM_VAR(v.texcoords);
        }

        for (uint i: mesh.indices()) {
                IM_VAR(i);
        }
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
