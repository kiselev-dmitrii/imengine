#include "PolygonalEntity.h"
#include "../Scene.h"
#include <glm/gtx/transform.hpp>

namespace imEngine {

PolygonalEntity::PolygonalEntity(const Model &model, SceneObject *parent) :
        EntityAbstract(parent),
        m_model(model)
{ }

void PolygonalEntity::render() {
        const Mat4& viewMatrix = scene()->currentCamera()->worldToLocalMatrix();
        const Mat4& projectionMatrix = scene()->currentCamera()->viewToClipMatrix();
        const Mat4& modelMatrix = localToWorldMatrix();

        Mat4 modelViewProjectionMatrix = projectionMatrix * viewMatrix * modelMatrix;

        for (const ModelDetail& detail: m_model.details()) {
                detail.material->bind();
                detail.material->program()->setUniform("uModelViewProjectionMatrix", modelViewProjectionMatrix);
                detail.geometry->render();
        }
}

} //namespace imEngine
