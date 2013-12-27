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
        auto tex = resources()->textures()->texture2D("energy.png");
        auto tex1 = resources()->textures()->texture2D("energy.png");
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
