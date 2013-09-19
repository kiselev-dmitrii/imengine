#include <imCore/System/BaseApplication.h>
#include <imCore/Graphics/Texture/Texture1D.h>
#include <imCore/Utils/GLUtils.h>
#include <imCore/Utils/Debug.h>

using namespace imCore;

class Application : public BaseApplication {
public:
        void    initialize();
        void    update();
        void    render();
        void    destroy();

private:
        Texture1D*       m_texture;
};

void Application::initialize() {
        m_texture = new Texture1D("resources/texture/texture_1d_test.png");
        m_texture->save("resources/texture/out.png");

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
