#include "Entity.h"
#include "../../ResourceManager.h"
#include "../../Scene.h"

namespace imEngine {

Entity::Entity(const String &model, Object *parent) :
        Movable(parent),
        m_model(RESOURCES->models()->model(model))
{
        m_type = ObjectType::ENTITY;

        m_model.setOwner(this);
        m_aabb = m_model.aabb();
        scene()->registerEntity(this);
}

Entity::~Entity() {
        scene()->unregisterEntity(this);
}

void Entity::loadFromJson(const JsonValue &node) {
        JsonValue materials = node["materials"];

        for (JsonValue material : materials)  {
                String detailName = material["name"].asString();
                Detail* detail = model()->detail(detailName);
                if (!detail) continue;

                MaterialPtr mat = Model::createMaterial(material);
                mat->loadFromJson(material);
                detail->setMaterial(mat);
        }

        Movable::loadFromJson(node);
}

void Entity::saveAsJson(JsonValue &result) {
        IM_TODO;
        Movable::saveAsJson(result);
}


} //namespace imEngine
