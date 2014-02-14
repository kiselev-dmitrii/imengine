#include <imEngine/Application/GraphicApplication.h>
#include <imEngine/Graphics/Scene/Scene.h>
#include <imEngine/Utils/Debug.h>
#include <imEngine/Graphics/Scene/Entity/Polygonal.h>
#include <imEngine/Graphics/GUI/Picture.h>
#include <imEngine/Graphics/GAPI/Framebuffer/Framebuffer.h>
#include <imEngine/Graphics/RenderTarget.h>
#include <imEngine/Graphics/Scene/PostEffects/Blur.h>
#include <imEngine/Graphics/PrimitiveRenderer.h>

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

        RenderTarget*           m_rt1;
        RenderTarget*           m_rt2;
        GeometryPtr             m_quad;

        HBlur*                  m_hblur;
        VBlur*                  m_vblur;

        Picture*                m_picture;

        VertexArrayPtr          m_vao;
        VertexBufferPtr         m_vbo;
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

        m_quad = Geometry::plane();
        m_hblur = new HBlur();
        m_hblur->setSigma(20.0);
        m_vblur = new VBlur();
        m_vblur->setSigma(20.0);

        /// Buffers
        m_vbo = VertexBufferPtr(new VertexBuffer());
        m_vao = VertexArrayPtr(new VertexArray());
        m_vao->bind();
                m_vbo->connect(0, 2, GL_FLOAT, 0, 0);
        m_vao->unbind();

        Vec2 lb(-1, -1);
        Vec2 rb(1, -1);
        Vec2 lt(-1, 1);
        Vec2 rt(1, 1);
        Vec2 data[] = {lb, rb, lt, rt};
        m_vbo->load(data, sizeof(data), BufferUsage::DYNAMIC_DRAW);

        /// FBO
        m_rt1 = new RenderTarget(mainWindow()->size()/4);
        m_rt1->enableColorBuffer(0, InternalFormat::COLOR_NORM_3_COMP_8_BIT, true);
        m_rt1->enableDepthBuffer(InternalFormat::DEPTH_NORM_1_COMP_16_BIT, false);

        m_rt2 = new RenderTarget(m_rt1->size());
        m_rt2->enableColorBuffer(0, InternalFormat::COLOR_NORM_3_COMP_8_BIT, true);

        m_picture = new Picture(m_rt1->colorBufferTexture(0), gui()->root());
        m_picture->setPosition(Vec2(0, 30));

}

void Application::render() {
        m_rt1->bind();
                Renderer::clearBuffers();
                scene()->setCurrentCamera(m_firstCamera);
                scene()->processRender();
        m_rt1->unbind();

        m_rt2->bind();
                Renderer::clearBuffers();
                m_hblur->setTexture(m_rt1->colorBufferTexture(0).get());
                m_hblur->bind();
                        m_vao->bind();
                        Renderer::renderVertices(Primitive::TRIANGLE_STRIP, 4);
                m_hblur->unbind();
        m_rt2->unbind();

        m_rt1->bind();
                Renderer::clearBuffers();
                m_vblur->setTexture(m_rt2->colorBufferTexture(0).get());
                m_vblur->bind();
                        m_vao->bind();
                        Renderer::renderVertices(Primitive::TRIANGLE_STRIP, 4);
                m_vblur->unbind();
        m_rt1->unbind();

        Renderer::clearBuffers();
        scene()->setCurrentCamera(m_secondCamera);
        scene()->processRender();

        gui()->processRender();
}

void Application::keyPressEvent(int key) {
        GraphicApplication::keyPressEvent(key);
        if (key == 'r') std::swap(m_firstCamera, m_secondCamera);
}

void Application::destroy() {
        GraphicApplication::destroy();

        delete m_rt1;
}

int main() {
        Application app;
        return app.exec();
}
