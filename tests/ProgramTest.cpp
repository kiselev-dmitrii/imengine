#include <imCore/Graphics/Shader/Program.h>
#include <imCore/System/BaseApplication.h>
#include <imCore/Utils/StringUtils.h>
#include <imCore/Utils/Debug.h>
#include "showTriangle.glsl"

using namespace imCore;

class Application : public BaseApplication {
public:
        void    initialize();
        void    update();
        void    render();
        void    destroy();

        void    keyPressEvent(int key);
private:
        Program*        m_redTriangle;
        GLuint          m_vao;
        GLuint          m_vbo;

};

void Application::initialize() {
        glClearColor(0,0,0,0);

        /// Программа
        m_redTriangle = new Program();
        m_redTriangle->addShaders(showTriangleSource);
        m_redTriangle->setMacroDefines({"RED"});
        m_redTriangle->build();

        /// VAO & VBO
        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);

        static const GLfloat triangle[] = {
                -1.0f, -1.0f, 0.0f,
                 1.0f, -1.0f, 0.0f,
                 0.0f,  1.0f, 0.0f,
        };

        glGenBuffers(1, &m_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);

        glEnableVertexAttribArray(ProgramAttributeLocations::POSITION);
        glVertexAttribPointer(ProgramAttributeLocations::POSITION, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*) nullptr);

}

void Application::update() {

}

void Application::render() {
        glClear(GL_COLOR_BUFFER_BIT);
        m_redTriangle->bind();

        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glDrawArrays(GL_TRIANGLES, 0, 3);

}

void Application::destroy() {
        delete m_redTriangle;
}

void Application::keyPressEvent(int key) {
        if (key == SDLK_r) {
                m_redTriangle->setMacroDefines({"GREEN"});
                m_redTriangle->build();
        }
}


int main() {
        Application app;
        return app.exec();
}
