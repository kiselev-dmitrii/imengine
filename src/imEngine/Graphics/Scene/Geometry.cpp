#include "Geometry.h"
#include <imEngine/Utils/Debug.h>

namespace imEngine {

Geometry::Geometry(const Mesh &mesh) :
        m_numVertices(0),
        m_numIndices(0),
        m_areVerticesLoaded(false)
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

float Geometry::radius() const {
        return m_radius;
}

void Geometry::transform(const Mat4 &matrix) {
        IM_ASSERT(m_areVerticesLoaded);

        Vertex* vertices = (Vertex*) m_vbo->map(BufferAccess::READ_WRITE);
                Mat3 normalMatrix = glm::transpose(Mat3(glm::inverse(matrix)));

                for (uint i = 0; i < m_numVertices; ++i) {
                        vertices[i].position = Vec3(matrix * Vec4(vertices[i].position, 1.0));
                        vertices[i].normal = glm::normalize(normalMatrix * vertices[i].normal);
                }
        m_vbo->unmap();

        calculateGeometryInformation();
}

void Geometry::load(const Mesh &mesh) {
        m_numVertices = mesh.vertices().size();
        m_numIndices = mesh.indices().size();

        m_vbo->load(&(mesh.vertices()[0]), m_numVertices*sizeof(Vertex), BufferUsage::STATIC_DRAW);
        m_ibo->load(&(mesh.indices()[0]), m_numIndices*sizeof(uint), BufferUsage::STATIC_DRAW);
        m_areVerticesLoaded = true;

        calculateGeometryInformation();
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

void Geometry::calculateGeometryInformation() {
        IM_ASSERT(m_areVerticesLoaded);

        AABB aabb;
        float radius;
        Vec3 sum;

        Vertex* vertices = (Vertex*) m_vbo->map(BufferAccess::READ_ONLY);
                // Начальный значения
                aabb.min = vertices[0].position;
                aabb.max = vertices[0].position;
                radius = 0;
                sum = Vec3(0,0,0);

                // Считаем
                for (uint i = 0; i < m_numVertices; ++i) {
                        aabb.min = glm::min(aabb.min, vertices[i].position);
                        aabb.max = glm::max(aabb.max, vertices[i].position);
                        radius = glm::max(radius, glm::length(vertices[i].position));
                        sum += vertices[i].position;
                }
        m_vbo->unmap();

        m_aabb = aabb;
        m_radius = radius;
        m_center = sum * (1.0f/m_numVertices);
}


} //namespace imEngine
