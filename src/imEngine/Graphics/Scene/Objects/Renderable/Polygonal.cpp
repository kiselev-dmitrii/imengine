#include "Polygonal.h"
#include "../../Scene.h"
#include <limits>

namespace imEngine {

Polygonal::Polygonal(const String &model, Object *parent) :
        Movable(parent),
        m_model(this)
{
        scene()->registerPolygonal(this);
        m_model.loadFromXML(model);
        calculateAABB();
}

Polygonal::Polygonal(const String &geometry, MaterialPtr material, Object* parent) :
        Movable(parent),
        m_model(geometry, material, this)
{
        scene()->registerPolygonal(this);
        calculateAABB();
}

const AABB& Polygonal::aabb() const {
        return m_aabb;
}

Model& Polygonal::model() {
        return m_model;
}

void Polygonal::calculateAABB() {
        float minFloat = std::numeric_limits<float>::min();
        float maxFloat = std::numeric_limits<float>::max();
        m_aabb.max = Vec3(minFloat);
        m_aabb.min = Vec3(maxFloat);

        for (const ModelDetail& detail: m_model.details()) {
                AABB geometryAABB = detail.geometry->aabb();
                m_aabb.max = glm::max(m_aabb.max, geometryAABB.max);
                m_aabb.min = glm::min(m_aabb.min, geometryAABB.min);
        }
}

} //namespace imEngine
