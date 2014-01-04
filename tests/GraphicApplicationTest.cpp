#include <imEngine/Application/GraphicApplication.h>

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
        GraphicApplication app;
        return app.exec();
}
