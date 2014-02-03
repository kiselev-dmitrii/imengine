#include "Geometry.h"
#include <imEngine/Utils/Debug.h>

namespace imEngine {

Geometry::Geometry(const Mesh &mesh) :
        m_numVertices(0),
        m_numIndices(0)
{
        initBuffers();
        load(mesh);
}

const AABB& Geometry::aabb() const {
        return m_aabb;
}

const Vec3& Geometry::center() const {
        return m_center;
}

void Geometry::transform(const Mat4 &m) {
}

void Geometry::load(const Mesh &mesh) {
        m_numVertices = mesh.vertices().size();
        m_numIndices = mesh.indices().size();

        m_vbo->load(&(mesh.vertices()[0]), m_numVertices*sizeof(Vertex), BufferUsage::STATIC_DRAW);
        m_ibo->load(&(mesh.indices()[0]), m_numIndices*sizeof(uint), BufferUsage::STATIC_DRAW);
}

void Geometry::render() const {
        m_vao->bind();
        Renderer::renderIndices(Primitive::TRIANGLE_STRIP, m_numIndices);
}

void Geometry::initBuffers() {
        m_vbo = VertexBufferPtr(new VertexBuffer());
        m_ibo = IndexBufferPtr(new IndexBuffer());
        m_vao = VertexArrayPtr(new VertexArray());

        m_vao->bind();
                m_vbo->connect(0, 3, GL_FLOAT, offsetof(Vertex, position), sizeof(Vertex));
                m_vbo->connect(1, 3, GL_FLOAT, offsetof(Vertex, normal), sizeof(Vertex));
                m_vbo->connect(2, 2, GL_FLOAT, offsetof(Vertex, texcoords), sizeof(Vertex));

                m_ibo->bind();
        m_vao->unbind();
}

} //namespace imEngine
