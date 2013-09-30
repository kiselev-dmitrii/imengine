#include <imCore/System/BaseApplication.h>
#include <imCore/Graphics/Shader/Program.h>
#include <imCore/Graphics/Texture/Texture2D.h>
#include <imCore/Graphics/Buffer/VertexBuffer.h>

#include <imCore/Utils/GLUtils.h>
#include <imCore/Utils/Debug.h>

using namespace imCore;

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
        Program         m_program;
        Texture2D       m_texture;
        VertexBuffer    m_vbo;

};

void Application::createProgram() {
        m_program.create();
        m_program.loadSourceFromFile("resources/shaders/test/billboards.glsl");
        m_program.build();
}

void Application::createTexture() {
        m_texture.create();
        m_texture.load("resources/texture/energy.png");

        m_texture.setMagnificationFilter(TextureMagFilter::LINEAR);
        m_texture.setMinimizationFilter(TextureMinFilter::LINEAR);
}

void Application::createVBO() {
        const GLfloat points[] = {
                 0.0f,  0.0f, 0.0f,
                 0.0f,  0.1f, 0.0f,
                 0.1f,  0.1f, 0.0f,
        };

        m_vbo.create();
        m_vbo.load(points, sizeof(points), BufferUsage::STATIC_DRAW);
        m_vbo.connect(ProgramAttributeLocations::POSITION, 3, GL_FLOAT, 0, 0);
}

void Application::initialize() {
        glClearColor(0.0, 0.0, 0.0, 1.0);

        createProgram();
        createTexture();
        createVBO();

        m_program.bind();
        m_program.setUniform("halfSize", 0.2f);
        m_texture.bind(0);
        m_program.setUniform("billboardTex", 0);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void Application::update() {
}

void Application::render() {
        glClear(GL_COLOR_BUFFER_BIT);

        m_vbo.bind();
        glDrawArrays(GL_POINTS, 0, 3);
}

void Application::windowResizeEvent(int x, int y) {
        glViewport(0,0, x,y);
}

void Application::keyPressEvent(int key) {
        IVec2 m = mainWindow()->mouse()->position();
        IVec2 w = mainWindow()->size();
        float x = -1.0 + 2.0*m.x/w.x;
        float y = 1.0 - 2.0*m.y/w.y;


        switch (key) {
                case SDLK_1: {
                        IM_LOG("Mapping update");
                        GLfloat* pointer = (GLfloat*) m_vbo.map(BufferAccess::WRITE_ONLY);
                        pointer[3] = x;
                        pointer[4] = y;
                        m_vbo.unmap();
                        break;
                }

                case SDLK_2: {
                        IM_LOG("Replace update");
                        const GLfloat points[] = {x + 0.2f, y + 0.2f, 0.0f};
                        m_vbo.replace(6*sizeof(GLfloat), 3*sizeof(GLfloat), points);
                        break;
                }

                case SDLK_3: {
                        IM_LOG("Reload new vertices");
                        const GLfloat points[] = {
                                -0.3f,  0.3f, 0.0f,
                                0.1f,  0.4f, 0.0f,
                                0.3f,  -0.2f, 0.0f,
                        };
                        m_vbo.load(points, sizeof(points), BufferUsage::DYNAMIC_DRAW);
                        break;
                }

                case SDLK_4: {
                        IM_LOG("Info: ");
                        IM_VAR(m_vbo.handle());
                        IM_VAR(GLUtils::convertEnumToString(m_vbo.target()));
                        IM_VAR(m_vbo.size());
                        IM_VAR(GLUtils::convertEnumToString(m_vbo.usage()));
                        break;
                }
        }
}

void Application::destroy() {
        m_program.destroy();
        m_texture.destroy();
        m_program.destroy();
}

int main() {
        Application app;
        app.exec();
}
