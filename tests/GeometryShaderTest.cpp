#include <imCore/System/BaseApplication.h>
#include <imCore/Graphics/Shader/Program.h>
#include <imCore/Graphics/Texture/Texture2D.h>
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
        void    createVAO();

private:
        Program         m_program;
        Texture2D       m_texture;

        GLuint          m_vao;
        GLuint          m_vbo;

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

void Application::createVAO() {
        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);

        static const GLfloat points[] = {
                 0.0f,  0.0f, 0.0f,
                 0.0f,  0.1f, 0.0f,
                 0.1f,  0.1f, 0.0f,
        };

        glGenBuffers(1, &m_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

        glEnableVertexAttribArray(ProgramAttributeLocations::POSITION);
        glVertexAttribPointer(ProgramAttributeLocations::POSITION, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*) nullptr);

}

void Application::initialize() {
        glClearColor(0.0, 0.0, 0.0, 1.0);
        createProgram();
        createTexture();
        createVAO();

        m_program.bind();
        m_program.setUniform("halfSize", 0.2f);
        m_texture.bind(0);
        m_program.setUniform("billboardTex", 0);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void Application::update() {
        IVec2 m = mainWindow()->mouse()->position();
        IVec2 w = mainWindow()->size();

        float x = -1.0 + 2.0*m.x/w.x;
        float y = 1.0 - 2.0*m.y/w.y;

        const GLfloat points[] = {x, y, 0.0};
        glBufferSubData(GL_ARRAY_BUFFER, 3*sizeof(GLfloat), 3*sizeof(GLfloat), points);
}


void Application::render() {
        glClear(GL_COLOR_BUFFER_BIT);

        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glDrawArrays(GL_POINTS, 0, 3);
}

void Application::destroy() {
        m_program.destroy();
        m_texture.destroy();
}

void Application::windowResizeEvent(int x, int y) {
        glViewport(0,0, x,y);
}

void Application::keyPressEvent(int key) {
}

int main() {
        Application app;
        app.exec();
}
