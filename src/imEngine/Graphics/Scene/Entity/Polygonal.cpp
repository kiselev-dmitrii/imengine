#include "Polygonal.h"
#include "../Scene.h"

namespace imEngine {

Polygonal::Polygonal(const Model &model, SceneObject *parent) :
        Entity(parent),
        m_model(model)
{ }

void Polygonal::render() {
        const Mat4& viewMatrix = scene()->currentCamera()->worldToLocalMatrix();
        const Mat4& projectionMatrix = scene()->currentCamera()->viewToClipMatrix();
        const Mat4& modelMatrix = localToWorldMatrix();

        Mat4 modelViewProjectionMatrix = projectionMatrix * viewMatrix * modelMatrix;
        Mat4 modelViewMatrix = viewMatrix * modelMatrix;
        Mat3 normalMatrix = glm::transpose(Mat3(glm::inverse(modelViewMatrix)));

        for (const ModelDetail& detail: m_model.details()) {
                detail.material->bind();
                detail.material->program()->setUniform("uModelViewProjectionMatrix", modelViewProjectionMatrix);
                detail.material->program()->setUniform("uModelViewMatrix", modelViewMatrix);
                detail.material->program()->setUniform("uNormalMatrix", normalMatrix);
                detail.geometry->render();
        }
}

} //namespace imEngine
