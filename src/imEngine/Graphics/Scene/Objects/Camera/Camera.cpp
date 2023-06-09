#include "Camera.h"
#include "../../Scene.h"
#include "../../ResourceManager.h"
#include <imEngine/Utils/Debug.h>
#include <glm/gtx/transform.hpp>

namespace imEngine {


Camera::Camera(Object* parent) :
        Movable(parent),
        m_movementSpeed(5),
        m_rotationSpeed(40)
{
        m_type = ObjectType::CAMERA;

        scene()->registerCamera(this);
        initHelper();
}

void Camera::loadFromJson(const JsonValue &node) {
        JsonValue movementSpeed = node["movement_speed"];
        JsonValue rotationSpeed = node["rotation_speed"];

        if (!movementSpeed.isNull()) setMovementSpeed(movementSpeed.asFloat());
        if (!rotationSpeed.isNull()) setRotationSpeed(rotationSpeed.asFloat());

        Frustum::loadFromJson(node);
        Movable::loadFromJson(node);
}

void Camera::saveAsJson(JsonValue &result) {
        Movable::saveAsJson(result);
        Frustum::saveAsJson(result);

        result["movement_speed"] = movementSpeed();
        result["rotation_speed"] = rotationSpeed();
}

void Camera::renderHelper() {
        const Mat4& viewMatrix = scene()->activeCamera()->worldToLocalMatrix();
        const Mat4& projectionMatrix = scene()->activeCamera()->viewToClipMatrix();
        const Mat4& modelMatrix = localToWorldMatrix();
        Mat4 modelViewProjectionMatrix = projectionMatrix * viewMatrix * modelMatrix;

        m_program->bind();
        m_program->setUniform("uModelViewProjectionMatrix", modelViewProjectionMatrix);
        m_program->setUniform("uBorderWidth", 0.02f);
        m_program->setUniform("uScale", 1.0f);
        m_geometry->render();
}

void Camera::initHelper() {
        m_program = RESOURCES->programs()->program("GeometryHelper.glsl");

        /// проекционная матрица переводит из ViewFrustum в [1;-1]x[1;-1]x[1;-1]
        /// обратная будет, наоборот, переводить куб в усеченную пирамиду
        m_geometry = Geometry::cube();
        Mat4 invProjectionMatrix = glm::inverse(viewToClipMatrix());
        m_geometry->transform(invProjectionMatrix * glm::scale(Mat4(1), Vec3(0.9,0.9,0.9)));
}


} //namespace imEngine
