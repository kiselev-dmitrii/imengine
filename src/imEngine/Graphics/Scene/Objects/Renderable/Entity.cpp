#include "Entity.h"
#include "../../ResourceManager.h"
#include "../../Scene.h"

namespace imEngine {

Entity::Entity(const String &model, Object *parent) :
        Movable(parent),
        m_model(*(RESOURCES->models()->model(model)))
{
        m_model.setOwner(this);
        scene()->registerEntity(this);
}

Entity::Entity(const String &geometry, EntityMaterialPtr material, Object *parent) :
        Movable(parent),
        m_model(Model("unnamed", geometry, material))
{
        m_model.setOwner(this);
        scene()->registerEntity(this);
}

Entity::~Entity() {
        scene()->unregisterEntity(this);
}


} //namespace imEngine
