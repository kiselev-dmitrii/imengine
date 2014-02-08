#include <imEngine/Application/BaseApplication.h>
#include <imEngine/Graphics/Scene/Resources.h>
#include <imEngine/Utils/Debug.h>

using namespace imEngine;

class TestApplication : public BaseApplication {
public:
        TestApplication() : tex("resources/texture/") {}
protected:
        void    initialize();
        void    update();
        void    render();
        void    destroy();

private:
        TextureManager tex;

};

void TestApplication::initialize() {
        Texture2D* texture1 = tex.texture2D("energy.png");
        texture1 = tex.texture2D("energy.png");
        IM_VAR(texture1->height());
        IM_VAR(texture1->width());
        IM_VAR(tex.list());
        IM_VAR(tex.directory());
        tex.reloadAll();
        tex.removeAll();
}

void TestApplication::update() {
}

void TestApplication::render() {
}

void TestApplication::destroy() {
}

int main() {
        TestApplication app;
        return app.exec();
}
