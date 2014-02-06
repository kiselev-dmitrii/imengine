#include "EntityAbstract.h"
#include "../Scene.h"

namespace imEngine {

EntityAbstract::EntityAbstract(SceneObject* parent) :
        SceneObject(parent)
{
        m_scene->addEntity(this);
}

EntityAbstract::~EntityAbstract() {
        m_scene->removeEntity(this);
}

} //namespace imEngine
