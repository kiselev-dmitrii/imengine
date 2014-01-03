#include <imEngine/Graphics/GAPI/Shader/Program.h>
#include <imEngine/Application/BaseApplication.h>
#include <imEngine/Utils/StringUtils.h>
#include <imEngine/Utils/Debug.h>
#include <cstdio>
#include "showTriangle.glsl"

using namespace imEngine;

class Application : public BaseApplication {
public:
        void    initialize();
        void    update();
        void    render();
        void    destroy();

        void    keyPressEvent(int key);

private:
        void    createVAO();
        void    createProgram1();
        void    createProgram2();

private:
        GLuint          m_vao;
        GLuint          m_vbo;

        Program*        m_program1;
        Program*        m_program2;

};

void Application::createVAO() {
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

void Application::createProgram1() {
        IM_LOG("Create Program1");
        m_program1 = new Program();
        m_program1->loadSource(showTriangleSource);
        m_program1->setDefines({"RED"});
        m_program1->build();
}

void Application::createProgram2() {
        IM_LOG("Create Program2");
        m_program2 = new Program();
        m_program2->loadSourceFromFile("resources/shaders/test/color.glsl");
        m_program2->build();

        m_program2->bind();
        m_program2->setUniform("uColor", Vec3(0.3, 0.3, 0.8));
        m_program2->unbind();
}

void Application::initialize() {
        glClearColor(0,0,0,0);
        createProgram1();
        createProgram2();
        createVAO();

}

void Application::update() {

}

void Application::render() {
        glClear(GL_COLOR_BUFFER_BIT);

        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glDrawArrays(GL_TRIANGLES, 0, 3);

}

void Application::destroy() {
        delete m_program1;
        delete m_program2;
}

void Application::keyPressEvent(int key) {
        switch (key) {
                case SDLK_1: {
                        IM_LOG("Program1 binding...");
                        m_program1->bind();
                        break;
                }
                case SDLK_2: {
                        IM_LOG("Program2 binding...");
                        m_program2->bind();
                        break;
                }

                case SDLK_3: {
                        IM_LOG("Program2: set uniform");
                        float r = (float)rand()/(float)RAND_MAX;
                        float g = (float)rand()/(float)RAND_MAX;
                        float b = (float)rand()/(float)RAND_MAX;
                        m_program2->bind();
                        m_program2->setUniform("uColor", Vec3(r, g, b));
                        break;
                }
                case SDLK_4: {
                        IM_LOG("Program1 rebuilding...");
                        m_program1->setDefines({"GREEN"});
                        m_program1->build();
                        break;
                }
                case SDLK_5: {
                        IM_LOG("Program2 rebuilding...");
                        m_program2->build();
                        break;
                }
                case SDLK_6: {
                        IM_LOG("Program1 info...");
                        IM_VAR(m_program1->handle());
                        IM_VAR(m_program1->source(ShaderType::VERTEX));
                        IM_VAR(m_program1->source(ShaderType::FRAGMENT));
                        IM_VAR(m_program1->defines());
                        IM_VAR(m_program1->log());
                        IM_VAR(m_program1->attributeLocation("im_vPosition"));
                        break;
                }
        }


}


int main() {
        Application app;
        return app.exec();
}
