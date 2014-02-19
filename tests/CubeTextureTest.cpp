#include <imEngine/Application/GraphicApplication.h>
#include <imEngine/Utils/Debug.h>
#include <imEngine/Utils/GLUtils.h>
#include <imEngine/Graphics/GUI/Picture.h>
#include <imEngine/Graphics/GUI/Panel.h>

using namespace imEngine;

class Application : public GraphicApplication {
protected:
        void    initialize();
        void    destroy();

private:
        Texture2DPtr    m_texture;
        CubeTexturePtr  m_cube;

        Panel*          m_panel;
        PictureColor*   m_envirenment;

};


void Application::initialize() {
        GraphicApplication::initialize();

        m_texture = Texture2DPtr(new Texture2D());
        m_texture->load("resources/environments/stpeters.env.hdr");
        m_texture->setMinimizationFilter(TextureMinFilter::LINEAR);
        m_texture->setMagnificationFilter(TextureMagFilter::LINEAR);
        IM_VAR(m_texture->width());
        IM_VAR(m_texture->height());
        IM_VAR(GLUtils::convertEnumToString(m_texture->sourceFormat()));
        IM_VAR(GLUtils::convertEnumToString(m_texture->sourceType()));
        IM_VAR(GLUtils::convertEnumToString(m_texture->internalFormat()));

        m_panel = new Panel("regular_panel.png", gui()->root());
        m_panel->setSize(Vec2(m_texture->width(), m_texture->height()));
        m_envirenment = new PictureColor(m_texture, m_panel);

        m_cube = CubeTexturePtr(new CubeTexture());
        m_cube->load("resources/environments/stpeters.env.hdr");
        IM_VAR(m_cube->width());
        IM_VAR(m_cube->height());
        IM_VAR(GLUtils::convertEnumToString(m_cube->sourceFormat()));
        IM_VAR(GLUtils::convertEnumToString(m_cube->sourceType()));
        IM_VAR(GLUtils::convertEnumToString(m_cube->internalFormat()));

}

void Application::destroy() {
        delete m_envirenment;
}

int main() {
        Application app;
        return app.exec();
}
