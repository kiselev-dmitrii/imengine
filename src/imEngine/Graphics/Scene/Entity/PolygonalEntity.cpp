#include "PolygonalEntity.h"
#include "../Scene.h"

namespace imEngine {

PolygonalEntity::PolygonalEntity(SceneObject *parent) :
        EntityAbstract(parent)
{
        m_geometry = Geometry::cube();
        m_program = ProgramPtr(new Program());
        m_program->loadSourceFromFile("resources/shaders/showGeometry.glsl");
        m_program->build();
}

void PolygonalEntity::render() {
        const Mat4& viewMatrix = scene()->currentCamera()->worldToLocalMatrix();
        const Mat4& projectionMatrix = scene()->currentCamera()->viewToClipMatrix();
        const Mat4& modelMatrix = localToWorldMatrix();

        Mat4 modelViewProjectionMatrix = projectionMatrix * viewMatrix * modelMatrix;

        m_program->bind();
        m_program->setUniform("uModelViewProjectionMatrix", modelViewProjectionMatrix);
        m_program->setUniform("uBorderWidth", 0.02f);
        m_program->setUniform("uScale", 1.0f);
        m_geometry->render();
}

} //namespace imEngine
