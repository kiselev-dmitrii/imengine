#include <imEngine/Application/BaseApplication.h>
#include <imEngine/Graphics/GAPI/Texture/Texture2D.h>
#include <imEngine/Utils/GLUtils.h>
#include <imEngine/Utils/Debug.h>

using namespace imEngine;
using namespace GAPI;

class Application : public BaseApplication {
public:
        void    initialize();
        void    update();
        void    render();
        void    destroy();

private:
        GAPI::Texture2D       m_texture1;
        GAPI::Texture2D       m_texture2;
};

void Application::initialize() {
        m_texture1.create();
        m_texture1.load("resources/texture/texture_2d_test.jpg");
        m_texture1.save("resources/texture/texture_2d_out.jpg");

        IM_VAR(m_texture1.width());
        IM_VAR(m_texture1.height());
        IM_VAR(m_texture1.depth());
        IM_VAR(m_texture1.wasMemoryAllocated());
        IM_VAR(m_texture1.sizeOfData());
        IM_VAR(GLUtils::convertEnumToString(m_texture1.sourceFormat()));
        IM_VAR(GLUtils::convertEnumToString(m_texture1.sourceType()));
        IM_VAR(GLUtils::convertEnumToString(m_texture1.internalFormat()));

        m_texture2.create();
        m_texture2.allocate(512, 512, TextureInternalFormat::COLOR_FLOAT_3_COMP_32_BIT, TextureSrcType::FLOAT, TextureSrcFormat::RGB);
        m_texture2.save("resources/texture/texture_2d_empty_out.raw");
        IM_VAR(m_texture2.width());
        IM_VAR(m_texture2.height());
        IM_VAR(m_texture2.depth());
        IM_VAR(m_texture2.wasMemoryAllocated());
        IM_VAR(m_texture2.sizeOfData());
        IM_VAR(GLUtils::convertEnumToString(m_texture2.sourceFormat()));
        IM_VAR(GLUtils::convertEnumToString(m_texture2.sourceType()));
        IM_VAR(GLUtils::convertEnumToString(m_texture2.internalFormat()));
}

void Application::update() {

}

void Application::render() {

}

void Application::destroy() {
        m_texture1.destroy();
        m_texture2.destroy();
}

int main() {
        Application app;
        return app.exec();
}
