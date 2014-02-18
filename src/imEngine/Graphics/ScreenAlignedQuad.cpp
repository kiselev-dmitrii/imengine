#include "ScreenAlignedQuad.h"

namespace imEngine {


ScreenAlignedQuad::ScreenAlignedQuad() {
        initBuffers();
        loadData();
}

void ScreenAlignedQuad::render() {
        m_vao->bind();
        Renderer::renderVertices(Primitive::TRIANGLE_STRIP, 4);
}

void ScreenAlignedQuad::initBuffers() {
        m_vbo = VertexBufferPtr(new VertexBuffer());
        m_vao = VertexArrayPtr(new VertexArray());
        m_vao->bind();
                m_vbo->connect(0, 2, GL_FLOAT, 0, 0);
        m_vao->unbind();
}

void ScreenAlignedQuad::loadData() {
         Vec2 data[] = {
                Vec2(-1, -1),
                Vec2(1, -1),
                Vec2(-1, 1),
                Vec2(1, 1)
        };
        m_vbo->load(data, sizeof(data), BufferUsage::DYNAMIC_DRAW);
}


} //namespace imEngine
