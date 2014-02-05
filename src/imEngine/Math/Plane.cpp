#include "Plane.h"

namespace imEngine {

Plane::Plane() {
        m_normal = Vec3(0,0,1);
        m_offset = 0;
}

Plane::Plane(float a, float b, float c, float d) {
        m_normal = Vec3(a,b,c);
        m_offset = d;

        m_offset /= m_normal.length();
        m_normal /= m_normal.length();
}

 Plane::Plane(const Vec3 &p0, const Vec3 &p1, const Vec3 &p2) {
        Vec3 u(p1 - p0);
        Vec3 v(p2 - p0);
        m_normal = glm::normalize(glm::cross(u, v));

        // Ax + By + Cz = D
        // D = (Ax + By + Cz) = dot(n, anyPlanePoint);
        m_offset = glm::dot(m_normal, p0);
}

Plane::Plane(const Vec3 &origin, const Vec3 &normal) {
        m_normal = normal;
        m_offset = glm::dot(normal, origin);
}

void Plane::setNormal(const Vec3 &normal) {
        m_normal = normal;
}

Vec3 Plane::normal() const {
        return m_normal;
}

void Plane::setOffset(float d) {
        m_offset = d;
}

float Plane::offset() const {
        return m_offset;
}

void Plane::setOrigin(const Vec3 &origin) {
        m_offset = glm::dot(m_normal, origin);
}

float Plane::distance(const Vec3& point) const {
        return glm::dot(m_normal, point) - m_offset;
}

Vec3 Plane::closest(const Vec3 &point) const {
        return point - m_normal * distance(point);
}


} //namespace imEngine
