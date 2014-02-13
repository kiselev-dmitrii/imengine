#include <imEngine/Application/GraphicApplication.h>
#include <imEngine/Graphics/Scene/Scene.h>
#include <imEngine/Utils/Debug.h>
#include <imEngine/Graphics/Scene/Entity/Polygonal.h>
#include <imEngine/Graphics/GUI/Picture.h>
#include <imEngine/Graphics/GAPI/Framebuffer/Framebuffer.h>
#include <imEngine/Graphics/RenderTarget.h>

using namespace imEngine;

class Application : public GraphicApplication {
protected:
        void    initialize();
        void    render();
        void    destroy();

        void    keyPressEvent(int key);
private:
        Polygonal*        m_car;
        Polygonal*        m_wheel1;

        CameraAbstract*         m_firstCamera;
        CameraAbstract*         m_secondCamera;

        RenderTarget*           m_renderTarget;
        Framebuffer*            m_fbo;
        Texture2DPtr            m_diffuseAttachment;
        Texture2DPtr            m_specularAttachment;
        Texture2DPtr            m_normalAttachment;
        Texture2DPtr            m_depthAttachment;

        Picture*                m_picture;
};


void Application::initialize() {
        GraphicApplication::initialize();

        Renderer::setBlendMode(BlendMode::ALPHA);
        Renderer::setDepthMode(DepthMode::LESS);
        m_car = new Polygonal(Model("resources/models/car.xml"), scene()->world());
        m_wheel1 = new Polygonal(Model("resources/models/wheel.xml"), m_car);
        m_wheel1->setPosition(Vec3(1.05,-0.7,0));

        m_firstCamera = scene()->currentCamera();
        m_secondCamera = new FirstPersonCamera(scene()->world());
        scene()->setCurrentCamera(m_secondCamera);

        /// FBO
        m_renderTarget = new RenderTarget(mainWindow()->size()/4);
        m_renderTarget->enableColorBuffer(0, InternalFormat::COLOR_NORM_3_COMP_8_BIT, true);
        m_renderTarget->enableColorBuffer(1, InternalFormat::COLOR_NORM_3_COMP_8_BIT, true);
        m_renderTarget->enableColorBuffer(2, InternalFormat::COLOR_NORM_3_COMP_8_BIT, true);
        m_renderTarget->enableDepthBuffer(InternalFormat::DEPTH_NORM_1_COMP_24_BIT, true);

        m_picture = new Picture(m_renderTarget->colorBufferTexture(0), gui()->root());
        m_picture->setPosition(Vec2(0, 30));

}

void Application::render() {
        m_renderTarget->bind();
                Renderer::clearBuffers();
                scene()->setCurrentCamera(m_firstCamera);
                scene()->processRender();
        m_renderTarget->unbind();

        Renderer::clearBuffers();
        scene()->setCurrentCamera(m_secondCamera);
        scene()->processRender();

        gui()->processRender();
}

void Application::keyPressEvent(int key) {
        GraphicApplication::keyPressEvent(key);
        if (key == 'r') std::swap(m_firstCamera, m_secondCamera);
        if (key == '1') m_picture->setTexture(m_renderTarget->colorBufferTexture(0));
        if (key == '2') m_picture->setTexture(m_renderTarget->colorBufferTexture(1));
        if (key == '3') m_picture->setTexture(m_renderTarget->colorBufferTexture(2));
}

void Application::destroy() {
        GraphicApplication::destroy();

        delete m_renderTarget;
}

int main() {
        Application app;
        return app.exec();
}
