#include "Entity.h"
#include "../Scene.h"

namespace imEngine {

Entity::Entity(SceneObject* parent) :
        SceneObject(parent)
{
        m_scene->addEntity(this);
}

Entity::~Entity() {
        m_scene->removeEntity(this);
}

} //namespace imEngine
