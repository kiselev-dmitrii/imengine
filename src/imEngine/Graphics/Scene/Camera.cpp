#include "Camera.h"
#include <imEngine/Utils/Debug.h>
#include "Scene.h"
#include <glm/gtx/transform.hpp>

namespace imEngine {


//############################## CameraAbstract ##############################//

CameraAbstract::CameraAbstract(SceneObject* parent) :
        SceneObject(parent),
        m_movementSpeed(5),
        m_rotationSpeed(40)
{
        m_scene->addCamera(this);

        m_program = ProgramPtr(new Program());
        m_program->loadSourceFromFile("resources/shaders/showGeometry.glsl");
        m_program->build();

        m_geometry = Geometry::cube();
        Mat4 invProjectionMatrix = glm::inverse(viewToClipMatrix());
        m_geometry->transform(invProjectionMatrix * glm::scale(Mat4(1), Vec3(0.9,0.9,0.9)));
}

void CameraAbstract::setMovementSpeed(float speed) {
        m_movementSpeed = speed;
}

void CameraAbstract::setRotationSpeed(float speed) {
        m_rotationSpeed = speed;
}

void CameraAbstract::renderHelper() {
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

//############################# FirstPersonCamera ############################//

FirstPersonCamera::FirstPersonCamera(SceneObject *parent) :
        CameraAbstract(parent)
{}

FirstPersonCamera::FirstPersonCamera(const Vec3 &psPosition, const Vec3 &psUp, const Vec3 &psRight, SceneObject *parent) :
        CameraAbstract(parent)
{
        setPosition(psPosition);
        setOrientation(psRight, psUp);
}

void FirstPersonCamera::moveForward(float delta) {
        translate(Vec3(0,0,-delta), Space::LOCAL);
}

void FirstPersonCamera::moveRight(float delta) {
        translate(Vec3(delta,0,0), Space::LOCAL);
}

void FirstPersonCamera::moveUp(float delta) {
        translate(Vec3(0,delta,0), Space::LOCAL);
}

void FirstPersonCamera::rotateHorizontaly(float angle) {
        rotate(Vec3(0,0,1), angle, Space::PARENT);
}

void FirstPersonCamera::rotateVerticaly(float angle) {
        rotate(Vec3(1,0,0), angle, Space::LOCAL);
}

void FirstPersonCamera::update(float deltaTime) {
        updatePosition(deltaTime);
        updateOrientation(deltaTime);
}

void FirstPersonCamera::updatePosition(float deltaTime) {
        Keyboard* keyboard = scene()->application()->mainWindow()->keyboard();

        if(keyboard->isKeyPressed(SDLK_w)) moveForward(m_movementSpeed * deltaTime);
        if(keyboard->isKeyPressed(SDLK_s)) moveForward(-m_movementSpeed * deltaTime);
        if(keyboard->isKeyPressed(SDLK_d)) moveRight(m_movementSpeed * deltaTime);
        if(keyboard->isKeyPressed(SDLK_a)) moveRight(-m_movementSpeed * deltaTime);
}

void FirstPersonCamera::updateOrientation(float deltaTime) {
        GraphicApplication* app = scene()->application();
        Mouse* mouse = app->mainWindow()->mouse();

        IVec2 position = mouse->position();
        mouse->setPosition(app->mainWindow()->center());
        Vec2 delta = Vec2(app->mainWindow()->center() - position);

        Vec2 angles = delta * m_rotationSpeed * deltaTime;
        rotateHorizontaly(angles.x);
        rotateVerticaly(angles.y);
}

} //namespace imEngine
