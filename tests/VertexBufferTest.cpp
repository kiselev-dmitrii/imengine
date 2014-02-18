#include <imEngine/Application/BaseApplication.h>
#include <imEngine/Graphics/GAPI/Shader/Program.h>
#include <imEngine/Graphics/GAPI/Texture/Texture2D.h>
#include <imEngine/Graphics/GAPI/Buffer/VertexBuffer.h>
#include <imEngine/Graphics/GAPI/Render/Renderer.h>

#include <imEngine/Utils/GLUtils.h>
#include <imEngine/Utils/Debug.h>

using namespace imEngine;

class Application : public BaseApplication {
protected:
        void    initialize();
        void    update();
        void    render();
        void    destroy();

        void    windowResizeEvent(int x, int y);
        void    keyPressEvent(int key);

private:
        void    createProgram();
        void    createTexture();
        void    createVBO();

private:
        Program*        m_program;
        Texture2D*      m_texture;
        VertexBuffer*   m_vbo;

};

void Application::createProgram() {
        m_program = new Program();
        m_program->loadSourceFromFile("resources/shaders/test/billboards.glsl");
        m_program->build();
}

void Application::createTexture() {
        m_texture = new Texture2D();
        m_texture->load("resources/texture/energy.png");

        m_texture->setMagnificationFilter(TextureMagFilter::LINEAR);
        m_texture->setMinimizationFilter(TextureMinFilter::LINEAR);
}

void Application::createVBO() {
        const GLfloat points[] = {
                 0.0f,  0.0f, 0.0f,
                 0.0f,  0.1f, 0.0f,
                 0.1f,  0.1f, 0.0f,
        };

        m_vbo = new VertexBuffer();
        m_vbo->load(points, sizeof(points), BufferUsage::STATIC_DRAW);
        m_vbo->connect(ProgramAttributeLocations::POSITION, 3, GL_FLOAT, 0, 0);
}

void Application::initialize() {
        glClearColor(0.0, 0.0, 0.0, 1.0);

        createProgram();
        createTexture();
        createVBO();

        m_program->bind();
        m_program->setUniform("halfSize", 0.2f);
        m_texture->bind(0);
        m_program->setUniform("billboardTex", 0);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void Application::update() {
}

void Application::render() {
        Renderer::clearBuffers();
        m_vbo->bind();
        Renderer::renderVertices(Primitive::POINT, 3);
}

void Application::windowResizeEvent(int x, int y) {
        glViewport(0,0, x,y);
}

void Application::keyPressEvent(int key) {
        IVec2 m = window()->mouse()->position();
        IVec2 w = window()->size();
        float x = -1.0 + 2.0*m.x/w.x;
        float y = 1.0 - 2.0*m.y/w.y;


        switch (key) {
                case SDLK_1: {
                        IM_LOG("Mapping update");
                        GLfloat* pointer = (GLfloat*) m_vbo->map(BufferAccess::WRITE_ONLY);
                        pointer[3] = x;
                        pointer[4] = y;
                        m_vbo->unmap();
                        break;
                }

                case SDLK_2: {
                        IM_LOG("Replace update");
                        const GLfloat points[] = {x + 0.2f, y + 0.2f, 0.0f};
                        m_vbo->replace(6*sizeof(GLfloat), 3*sizeof(GLfloat), points);
                        break;
                }

                case SDLK_3: {
                        IM_LOG("Reload new vertices");
                        const GLfloat points[] = {
                                -0.3f,  0.3f, 0.0f,
                                0.1f,  0.4f, 0.0f,
                                0.3f,  -0.2f, 0.0f,
                        };
                        m_vbo->load(points, sizeof(points), BufferUsage::DYNAMIC_DRAW);
                        break;
                }

                case SDLK_4: {
                        IM_LOG("Info: ");
                        IM_VAR(m_vbo->handle());
                        IM_VAR(GLUtils::convertEnumToString(m_vbo->target()));
                        IM_VAR(m_vbo->size());
                        IM_VAR(GLUtils::convertEnumToString(m_vbo->usage()));
                        break;
                }
        }
}

void Application::destroy() {
        delete m_program;
        delete m_texture;
        delete m_vbo;
}

int main() {
        Application app;
        app.exec();
}
