#include "PolygonalEntity.h"
#include "../Scene.h"

namespace imEngine {

PolygonalEntity::PolygonalEntity(SceneObject *parent) :
        EntityAbstract(parent)
{
        m_geometry = Geometry::plane();
        m_material = new WiredMaterial();
        m_material->setScale(10);
        m_material->setBorderWidth(0.005);
}

void PolygonalEntity::render() {
        const Mat4& viewMatrix = scene()->currentCamera()->worldToLocalMatrix();
        const Mat4& projectionMatrix = scene()->currentCamera()->viewToClipMatrix();
        const Mat4& modelMatrix = localToWorldMatrix();

        Mat4 modelViewProjectionMatrix = projectionMatrix * viewMatrix * modelMatrix;

        m_material->bind();
        m_material->program()->setUniform("uModelViewProjectionMatrix", modelViewProjectionMatrix);
        m_geometry->render();
}

} //namespace imEngine
