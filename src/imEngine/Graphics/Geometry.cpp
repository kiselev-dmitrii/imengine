#include "Geometry.h"
#include <imEngine/Utils/Debug.h>

namespace imEngine {

Geometry::Geometry(const VertexList &vertices, const IndexList &indices) :
        m_numVertices(0),
        m_numIndices(0),
        m_areVerticesLoaded(false)
{
        initBuffers();
        load(vertices, indices);
}

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

VertexList Geometry::vertices() {
        IM_ASSERT(m_areVerticesLoaded);

        VertexList result;
        result.reserve(m_numVertices);

        Vertex* vertices = (Vertex*) m_vbo->map(BufferAccess::READ_ONLY);
                for (uint i = 0; i < m_numVertices; ++i) result.push_back(vertices[i]);
        m_vbo->unmap();

        return result;
}

IndexList Geometry::indices() {
        IM_ASSERT(m_areVerticesLoaded);

        IndexList result;
        result.reserve(m_numIndices);

        uint* indices = (uint*) m_ibo->map(BufferAccess::READ_ONLY);
                for (uint i = 0; i < m_numIndices; ++i)  result.push_back(indices[i]);
        m_ibo->unmap();

        return result;
}

void Geometry::transform(const Mat4 &matrix) {
        IM_ASSERT(m_areVerticesLoaded);

        Vertex* vertices = (Vertex*) m_vbo->map(BufferAccess::READ_WRITE);
                Mat3 normalMatrix = glm::transpose(Mat3(glm::inverse(matrix)));

                for (uint i = 0; i < m_numVertices; ++i) {
                        Vec4 position = matrix * Vec4(vertices[i].position, 1.0);
                        vertices[i].position = Vec3(position)/position.w;

                        vertices[i].normal = glm::normalize(normalMatrix * vertices[i].normal);
                        vertices[i].tangent = glm::normalize(normalMatrix * vertices[i].tangent);
                }
        m_vbo->unmap();

        calculateGeometryInformation();
}

void Geometry::load(const VertexList &vertices, const IndexList &indices) {
        m_numVertices = vertices.size();
        m_numIndices = indices.size();

        m_vbo->load(&(vertices[0]), m_numVertices*sizeof(Vertex), BufferUsage::STATIC_DRAW);
        m_ibo->load(&(indices[0]), m_numIndices*sizeof(uint), BufferUsage::STATIC_DRAW);
        m_areVerticesLoaded = true;

        calculateGeometryInformation();
}

void Geometry::load(const Mesh &mesh) {
        load(mesh.vertices(), mesh.indices());
}

void Geometry::render() const {
        m_vao->bind();
        Renderer::renderIndices(Primitive::TRIANGLE, m_numIndices);
}

GeometryPtr Geometry::cube() {
        return box(Vec3(-1), Vec3(1));
}

GeometryPtr Geometry::box(const Vec3 &min, const Vec3 &max) {
        VertexList vertices = {
                { Vec3(min.x, min.y, min.z), Vec3(0, 0, -1), Vec2(0, 1) },
                { Vec3(max.x, max.y, min.z), Vec3(0, 0, -1), Vec2(1, 0) },
                { Vec3(max.x, min.y, min.z), Vec3(0, 0, -1), Vec2(1, 1) },
                { Vec3(min.x, max.y, min.z), Vec3(0, 0, -1), Vec2(0, 0) },
                { Vec3(min.x, min.y, min.z), Vec3(-1, 0, 0), Vec2(0, 1) },
                { Vec3(min.x, max.y, max.z), Vec3(-1, 0, 0), Vec2(1, 0) },
                { Vec3(min.x, max.y, min.z), Vec3(-1, 0, 0), Vec2(0, 0) },
                { Vec3(min.x, min.y, max.z), Vec3(-1, 0, 0), Vec2(1, 1) },
                { Vec3(min.x, max.y, min.z), Vec3(0, 1, 0), Vec2(0, 1)  },
                { Vec3(max.x, max.y, max.z), Vec3(0, 1, 0), Vec2(1, 0)  },
                { Vec3(max.x, max.y, min.z), Vec3(0, 1, 0), Vec2(0, 0)  },
                { Vec3(min.x, max.y, max.z), Vec3(0, 1, 0), Vec2(1, 1)  },
                { Vec3(max.x, min.y, min.z), Vec3(1, 0, 0), Vec2(1, 1)  },
                { Vec3(max.x, max.y, min.z), Vec3(1, 0, 0), Vec2(1, 0)  },
                { Vec3(max.x, max.y, max.z), Vec3(1, 0, 0), Vec2(0, 0)  },
                { Vec3(max.x, min.y, max.z), Vec3(1, 0, 0), Vec2(0, 1)  },
                { Vec3(min.x, min.y, min.z), Vec3(0, -1, 0), Vec2(0, 1) },
                { Vec3(max.x, min.y, min.z), Vec3(0, -1, 0), Vec2(1, 1) },
                { Vec3(max.x, min.y, max.z), Vec3(0, -1, 0), Vec2(1, 0) },
                { Vec3(min.x, min.y, max.z), Vec3(0, -1, 0), Vec2(0, 0) },
                { Vec3(min.x, min.y, max.z), Vec3(0, 0, 1), Vec2(0, 1)  },
                { Vec3(max.x, min.y, max.z), Vec3(0, 0, 1), Vec2(1, 1)  },
                { Vec3(max.x, max.y, max.z), Vec3(0, 0, 1), Vec2(1, 0)  },
                { Vec3(min.x, max.y, max.z), Vec3(0, 0, 1), Vec2(0, 0)  }
        };

        IndexList indices = {
                0,1,2,0,3,1,4,5,6,4,7,5,8,9,10,8,11,9,12,13,14,12,14,15,16,17,18,16,18,19,20,21,22,20,22,23
        };

        return GeometryPtr(new Geometry(vertices, indices));
}

GeometryPtr Geometry::plane() {
        VertexList vertices = {
                { Vec3(-1, -1, 0), Vec3(0, 0, 1), Vec2(0, 0) },
                { Vec3(1, 1, 0), Vec3(0, 0, 1), Vec2(1, 1) },
                { Vec3(-1, 1, 0), Vec3(0, 0, 1), Vec2(0, 1) },
                { Vec3(1, -1, 0), Vec3(0, 0, 1), Vec2(1, 0) }
        };

        IndexList indices = {
                0, 1, 2, 0, 3, 1
        };

        return GeometryPtr(new Geometry(vertices, indices));
}

void Geometry::initBuffers() {
        m_vbo = VertexBufferPtr(new VertexBuffer());
        m_ibo = IndexBufferPtr(new IndexBuffer());
        m_vao = VertexArrayPtr(new VertexArray());

        m_vao->bind();
                m_vbo->connect(0, 3, GL_FLOAT, offsetof(Vertex, position), sizeof(Vertex));
                m_vbo->connect(1, 3, GL_FLOAT, offsetof(Vertex, normal), sizeof(Vertex));
                m_vbo->connect(2, 2, GL_FLOAT, offsetof(Vertex, texcoords), sizeof(Vertex));
                m_vbo->connect(3, 3, GL_FLOAT, offsetof(Vertex, tangent), sizeof(Vertex));

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
