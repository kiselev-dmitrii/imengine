#include "Entity.h"
#include "../../ResourceManager.h"
#include "../../Scene.h"

namespace imEngine {

Entity::Entity(const String &model, Object *parent) :
        Movable(parent),
        m_model(RESOURCES->models()->model(model))
{
        m_model.setOwner(this);
        m_aabb = m_model.aabb();
        scene()->registerEntity(this);
}

Entity::~Entity() {
        scene()->unregisterEntity(this);
}


} //namespace imEngine
