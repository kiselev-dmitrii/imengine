#include "PolygonalEntity.h"
#include "../Scene.h"

namespace imEngine {

PolygonalEntity::PolygonalEntity(SceneObject *parent) :
        EntityAbstract(parent)
{
        m_model = new Model("resources/models/car.xml");
}

void PolygonalEntity::render() {
        m_model->render(scene()->currentCamera(), this);
}

} //namespace imEngine
