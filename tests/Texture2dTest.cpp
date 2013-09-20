#include <imCore/System/BaseApplication.h>
#include <imCore/Graphics/Texture/Texture2D.h>
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
        Texture2D*      m_texture1;
        Texture2D*      m_texture2;
};

void Application::initialize() {
        m_texture1 = new Texture2D("resources/texture/texture_2d_test.jpg");
        m_texture1->save("resources/texture/texture_2d_out.jpg");

        IM_VAR(m_texture1->width());
        IM_VAR(m_texture1->height());
        IM_VAR(m_texture1->depth());
        IM_VAR(m_texture1->wasMemoryAllocated());
        IM_VAR(m_texture1->sizeOfData());
        IM_VAR(GLUtils::convertEnumToString(m_texture1->sourceFormat()));
        IM_VAR(GLUtils::convertEnumToString(m_texture1->sourceType()));
        IM_VAR(GLUtils::convertEnumToString(m_texture1->internalFormat()));

        m_texture2 = new Texture2D(512, 512, TextureInternalFormat::COLOR_FLOAT_3_COMP_32_BIT, TextureSrcType::FLOAT, TextureSrcFormat::RGB);
        m_texture2->save("resources/texture/texture_2d_empty_out.raw");
        IM_VAR(m_texture2->width());
        IM_VAR(m_texture2->height());
        IM_VAR(m_texture2->depth());
        IM_VAR(m_texture2->wasMemoryAllocated());
        IM_VAR(m_texture2->sizeOfData());
        IM_VAR(GLUtils::convertEnumToString(m_texture2->sourceFormat()));
        IM_VAR(GLUtils::convertEnumToString(m_texture2->sourceType()));
        IM_VAR(GLUtils::convertEnumToString(m_texture2->internalFormat()));


}

void Application::update() {

}

void Application::render() {

}

void Application::destroy() {
        delete m_texture1;
        delete m_texture2;
}

int main() {
        Application app;
        return app.exec();
}
