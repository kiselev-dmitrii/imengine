#include "Camera.h"
#include <imEngine/Utils/Debug.h>

namespace imEngine {


//############################## CameraAbstract ##############################//

CameraAbstract::CameraAbstract(Movable* parent) :
        Movable(parent),
        m_movementSpeed(5),
        m_rotationSpeed(40)
{}

void CameraAbstract::setMovementSpeed(float speed) {
        m_movementSpeed = speed;
}

void CameraAbstract::setRotationSpeed(float speed) {
        m_rotationSpeed = speed;
}

void CameraAbstract::renderHelper() {
        IM_TODO;
}

//############################# FirstPersonCamera ############################//

FirstPersonCamera::FirstPersonCamera(Movable *parent) :
        CameraAbstract(parent)
{}

FirstPersonCamera::FirstPersonCamera(const Vec3 &psPosition, const Vec3 &psUp, const Vec3 &psRight, Movable *parent) :
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

void FirstPersonCamera::update(GraphicApplication *application, float deltaTime) {
        updatePosition(application, deltaTime);
        updateOrientation(application, deltaTime);
}

void FirstPersonCamera::updatePosition(GraphicApplication *application, float deltaTime) {
        Keyboard* keyboard = application->mainWindow()->keyboard();

        if(keyboard->isKeyPressed(SDLK_w)) moveForward(m_movementSpeed * deltaTime);
        if(keyboard->isKeyPressed(SDLK_s)) moveForward(-m_movementSpeed * deltaTime);
        if(keyboard->isKeyPressed(SDLK_d)) moveRight(m_movementSpeed * deltaTime);
        if(keyboard->isKeyPressed(SDLK_a)) moveRight(-m_movementSpeed * deltaTime);
}

void FirstPersonCamera::updateOrientation(GraphicApplication *application, float deltaTime) {
        Mouse* mouse = application->mainWindow()->mouse();

        IVec2 position = mouse->position();
        mouse->setPosition(application->mainWindow()->center());
        Vec2 delta = Vec2(application->mainWindow()->center() - position);

        Vec2 angles = delta * m_rotationSpeed * deltaTime;
        rotateHorizontaly(angles.x);
        rotateVerticaly(angles.y);
}

} //namespace imEngine
