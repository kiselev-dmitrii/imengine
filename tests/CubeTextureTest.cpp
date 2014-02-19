#include <imEngine/Application/GraphicApplication.h>
#include <imEngine/Utils/Debug.h>
#include <imEngine/Utils/GLUtils.h>
#include <imEngine/Graphics/GUI/Picture.h>

using namespace imEngine;

class Application : public GraphicApplication {
protected:
        void    initialize();
        void    destroy();

private:
        Texture2DPtr    m_texture;
        PictureColor*   m_envirenment;
};


void Application::initialize() {
        GraphicApplication::initialize();

        m_texture = Texture2DPtr(new Texture2D());
        m_texture->load("resources/environments/galileo.env.hdr");
        IM_VAR(m_texture->width());
        IM_VAR(m_texture->height());
        IM_VAR(GLUtils::convertEnumToString(m_texture->sourceFormat()));
        IM_VAR(GLUtils::convertEnumToString(m_texture->sourceType()));
        IM_VAR(GLUtils::convertEnumToString(m_texture->internalFormat()));
        m_envirenment = new PictureColor(m_texture, gui()->root());

}

void Application::destroy() {
        delete m_envirenment;
}

int main() {
        Application app;
        return app.exec();
}
