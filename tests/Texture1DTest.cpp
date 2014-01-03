#include <imEngine/Application/BaseApplication.h>
#include <imEngine/Graphics/GAPI/Texture/Texture1D.h>
#include <imEngine/Utils/GLUtils.h>
#include <imEngine/Utils/Debug.h>

using namespace imEngine;

class Application : public BaseApplication {
public:
        void    initialize();
        void    update();
        void    render();
        void    destroy();

private:
        Texture1D*      m_texture;
};

void Application::initialize() {
        m_texture = new Texture1D();
        m_texture->load("resources/texture/texture_1d_test.png");
        m_texture->save("resources/texture/out1.png");

        IM_VAR(m_texture->width());
        IM_VAR(m_texture->height());
        IM_VAR(m_texture->depth());
        IM_VAR(m_texture->wasMemoryAllocated());
        IM_VAR(GLUtils::convertEnumToString(m_texture->sourceFormat()));
        IM_VAR(GLUtils::convertEnumToString(m_texture->sourceType()));
        IM_VAR(GLUtils::convertEnumToString(m_texture->internalFormat()));
}

void Application::update() {

}

void Application::render() {

}

void Application::destroy() {
        delete m_texture;
}

int main() {
        Application app;
        return app.exec();
}
