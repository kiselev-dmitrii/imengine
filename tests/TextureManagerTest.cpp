#include <imEngine/Application/GraphicApplication.h>
#include <imEngine/Utils/Debug.h>

using namespace imEngine;

class TestApplication : public GraphicApplication {
protected:
        void    initialize();
        void    update();
        void    render();
        void    destroy();
};

void TestApplication::initialize() {
        GraphicApplication::initialize();

        IM_VAR(resources()->textures()->list());
}

void TestApplication::update() {
        GraphicApplication::update();
}

void TestApplication::render() {
        GraphicApplication::render();
}

void TestApplication::destroy() {
        GraphicApplication::destroy();
        resources()->textures()->removeAll();
}

int main() {
        TestApplication app;
        return app.exec();
}
