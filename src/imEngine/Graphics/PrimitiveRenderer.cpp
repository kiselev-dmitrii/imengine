#include "PrimitiveRenderer.h"

namespace imEngine {

PrimitiveRenderer& PrimitiveRenderer::instance() {
        static PrimitiveRenderer instance;
        return instance;
}

PrimitiveRenderer::PrimitiveRenderer() {
        initProgram();
        initBuffers();
}

void PrimitiveRenderer::drawRectangleInClipSpace(const Vec2 &min, const Vec2 &max, const Vec3 &color) {
        Vec2 lb(min.x, min.y);
        Vec2 rb(max.x, min.y);
        Vec2 lt(min.x, max.y);
        Vec2 rt(max.x, max.y);
        Vec2 data[] = {lb, rb, lt, rt};
        m_vbo->load(data, sizeof(data), BufferUsage::DYNAMIC_DRAW);

        m_program->bind();
        m_program->setUniform("u_color", color);
        m_vao->bind();
        Renderer::renderVertices(Primitive::TRIANGLE_STRIP, 4);
}

void PrimitiveRenderer::initProgram() {
        String src = R"(
                ///### VERTEX SHADER ###///
                in vec2 in_pos;
                void main() {
                        gl_Position = vec4(in_pos, 0.0, 1.0);
                }

                ///### FRAGMENT SHADER ###///
                layout (location = 0) out vec4 im_outColor;
                uniform vec3 u_color;
                void main() {
                        im_outColor = vec4(u_color, 1.0);
                }

        )";

        m_program = ProgramPtr(new Program());
        m_program->loadSource(src);
        m_program->build();
}

void PrimitiveRenderer::initBuffers() {
        m_vbo = VertexBufferPtr(new VertexBuffer());
        m_vao = VertexArrayPtr(new VertexArray());

        m_vao->bind();
                m_vbo->connect(0, 2, GL_FLOAT, 0, 0);
        m_vao->unbind();
}

} //namespace imEngine
