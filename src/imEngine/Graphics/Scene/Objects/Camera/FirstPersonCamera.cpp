#include "FirstPersonCamera.h"
#include "../../Scene.h"

namespace imEngine {


FirstPersonCamera::FirstPersonCamera(Object *parent) :
        Camera(parent)
{
        m_type = ObjectType::FIRST_PERSON_CAMERA;
}

FirstPersonCamera::FirstPersonCamera(const Vec3 &psPosition, const Vec3 &psUp, const Vec3 &psRight, Object* parent) :
        Camera(parent)
{
        m_type = ObjectType::FIRST_PERSON_CAMERA;

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
        rotate(Vec3(0,1,0), angle, Space::PARENT);
}

void FirstPersonCamera::rotateVerticaly(float angle) {
        rotate(Vec3(1,0,0), angle, Space::LOCAL);
}

void FirstPersonCamera::update(float deltaTime) {
        updatePosition(deltaTime);
        updateOrientation(deltaTime);
}

void FirstPersonCamera::updatePosition(float deltaTime) {
        Keyboard* keyboard = scene()->application()->window()->keyboard();

        if(keyboard->isKeyPressed(SDLK_w)) moveForward(m_movementSpeed * deltaTime);
        if(keyboard->isKeyPressed(SDLK_s)) moveForward(-m_movementSpeed * deltaTime);
        if(keyboard->isKeyPressed(SDLK_d)) moveRight(m_movementSpeed * deltaTime);
        if(keyboard->isKeyPressed(SDLK_a)) moveRight(-m_movementSpeed * deltaTime);
}

void FirstPersonCamera::updateOrientation(float deltaTime) {
        GraphicApplication* app = scene()->application();
        Mouse* mouse = app->window()->mouse();

        IVec2 position = mouse->position();
        mouse->setPosition(app->window()->center());
        Vec2 delta = Vec2(app->window()->center() - position);

        Vec2 angles = delta * m_rotationSpeed * deltaTime;
        rotateHorizontaly(angles.x);
        rotateVerticaly(angles.y);
}


} //namespace imEngine
