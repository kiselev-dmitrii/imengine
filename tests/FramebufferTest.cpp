#include <imEngine/Application/GraphicApplication.h>
#include <imEngine/Graphics/Scene/Scene.h>
#include <imEngine/Utils/Debug.h>
#include <imEngine/Graphics/Scene/Entity/Polygonal.h>
#include <imEngine/Graphics/GUI/Picture.h>
#include <imEngine/Graphics/GAPI/Framebuffer/Framebuffer.h>

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

        Framebuffer*            m_fbo;
        Texture2DPtr            m_colorAttachment;
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
        IVec2 winSize = mainWindow()->size();
        IVec2 fboSize = winSize/4;

        m_colorAttachment = Texture2DPtr(new Texture2D());
        m_colorAttachment->allocate(fboSize.x, fboSize.y, InternalFormat::COLOR_NORM_4_COMP_8_BIT, SourceType::UBYTE, SourceFormat::RGBA);

        m_depthAttachment = Texture2DPtr(new Texture2D());
        m_depthAttachment->allocate(fboSize.x, fboSize.y, InternalFormat::DEPTH_NORM_1_COMP_24_BIT, SourceType::UBYTE, SourceFormat::DEPTH);

        m_fbo = new Framebuffer();
        m_fbo->attachColorBuffer(0, m_colorAttachment.get());
        m_fbo->attachDepthBuffer(m_depthAttachment.get());

        if (!m_fbo->isValid()) {
                IM_ERROR("FBO is not valid");
                exit(-1);
        }

        m_picture = new Picture(m_colorAttachment, gui()->root());

}

void Application::render() {
        m_fbo->bind();
                Renderer::clearBuffers();

                IVec2 old = Renderer::viewportSize();
                Renderer::setViewportSize(IVec2(m_colorAttachment->width(), m_colorAttachment->height()));
                scene()->setCurrentCamera(m_firstCamera);
                scene()->processRender();
                Renderer::setViewportSize(old);
        m_fbo->unbind();

        Renderer::clearBuffers();
        scene()->setCurrentCamera(m_secondCamera);
        scene()->processRender();

        gui()->processRender();
}

void Application::keyPressEvent(int key) {
        GraphicApplication::keyPressEvent(key);
        if (key == '1') std::swap(m_firstCamera, m_secondCamera);
        if (key == '2') {
                if (m_picture->texture() == m_colorAttachment) m_picture->setTexture(m_depthAttachment);
                else m_picture->setTexture(m_colorAttachment);
        }
}

void Application::destroy() {
        GraphicApplication::destroy();

        delete m_fbo;
}

int main() {
        Application app;
        return app.exec();
}
