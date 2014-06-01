#include "Scene.h"
#include "Objects/Camera/FirstPersonCamera.h"
#include "SceneRenderer.h"
#include <glm/gtc/matrix_transform.hpp>
#include <imEngine/System/Filesystem.h>

namespace imEngine {

/** @brief Представляет собой кореневой объект сцены
 */
class World : public Object {
public:
        /// Конструктор
        World(Scene* scene) : Object(nullptr)                                   { m_scene = scene; }
};

//############################## Scene #######################################//

Scene::Scene(GraphicApplication *application) :
        m_application(application),
        m_world(nullptr),
        m_activeCamera(nullptr),
        m_isInputCaptured(false)
{
        m_world = new World(this);
        m_activeCamera = new FirstPersonCamera(m_world);
        m_sceneRenderer = new SceneRenderer(this);
}

Scene::~Scene() {
        delete m_world;
        delete m_sceneRenderer;
}

void Scene::loadScene(const String &filename) {
        String path = Filesystem::joinPath("resources/scenes", filename);
        Json::Value root = JsonUtils::loadFile(path);

        world()->loadFromJson(root["objects"]);
}

Object* Scene::pickObject(int x, int y) {
        float aspectRatio = activeCamera()->aspectRatio();
        float tanHalfFovy = glm::tan(glm::radians(activeCamera()->fieldOfView()/2));
        Vec2 winSize = Vec2(m_application->window()->size());

        /// Приводим экранные координаты к [-1;1]
        Vec2 m = Vec2(x,winSize.y - y)/winSize;
        m = 2.0f * m - Vec2(1.0);

        Vec3 vViewRay = Vec3(
                m.x * aspectRatio * tanHalfFovy,
                m.y * tanHalfFovy,
                -1
        );
        vViewRay = glm::normalize(vViewRay);

        Vec3 cameraForwardWS = activeCamera()->convertLocalToWorld(vViewRay) - activeCamera()->worldPosition();
        Vec3 positionWS = activeCamera()->worldPosition();


        /// Исключить объект, в котором мы сейчас находимся
        Object* exclude = nullptr;
        for (Object* entity: m_entities) {
                const Mat4& invModelMatrix = entity->worldToLocalMatrix();
                Vec3 modelSpacePosition = Vec3(invModelMatrix * Vec4(positionWS, 1.0));
                if (entity->aabb().doesContain(modelSpacePosition)) exclude = entity;
        }
        for (Object* entity: m_volumes) {
                const Mat4& invModelMatrix = entity->worldToLocalMatrix();
                Vec3 modelSpacePosition = Vec3(invModelMatrix * Vec4(positionWS, 1.0));
                if (entity->aabb().doesContain(modelSpacePosition)) exclude = entity;
        }

        while (glm::length(positionWS - cameraForwardWS) < 20.0) {
                for (Object* entity: m_entities) {
                        if (entity == exclude) continue;
                        const Mat4& invModelMatrix = entity->worldToLocalMatrix();
                        Vec3 modelSpacePosition = Vec3(invModelMatrix * Vec4(positionWS, 1.0));
                        if (entity->aabb().doesContain(modelSpacePosition)) return entity;
                }
                for (Object* entity: m_volumes) {
                        if (entity == exclude) continue;
                        const Mat4& invModelMatrix = entity->worldToLocalMatrix();
                        Vec3 modelSpacePosition = Vec3(invModelMatrix * Vec4(positionWS, 1.0));
                        if (entity->aabb().doesContain(modelSpacePosition)) return entity;
                }

                positionWS += 0.1f * cameraForwardWS;
        }
        return nullptr;
}

void Scene::render() {
        m_sceneRenderer->render();
}

void Scene::windowResizeEvent(int w, int h) {
        for (Camera* camera: m_cameras) camera->setAspectRatio(float(w)/h);
        m_activeCamera->setAspectRatio(float(w)/h);
        m_sceneRenderer->windowResizeEvent(w, h);
}

void Scene::update(float delta) {
        if (m_isInputCaptured) m_activeCamera->update(delta);
}

void Scene::mousePressEvent(int x, int y, char button) {
        Window *window = application()->window();
        Mouse *mouse = window->mouse();
        if (button != MouseButton::MIDDLE) return;

        static IVec2 oldMousePosition(0,0);
        if (!isInputCaptured()) {
                mouse->setVisible(false);
                oldMousePosition = mouse->position();
                mouse->setPosition(window->center());
                setInputCaptured(true);
        } else {
                mouse->setPosition(oldMousePosition);
                mouse->setVisible(true);
                setInputCaptured(false);
        }
}

void Scene::registerCamera(Camera *camera) {
        m_cameras.push_back(camera);
}

void Scene::unregisterCamera(Camera *camera) {
        auto it = std::find(m_cameras.begin(), m_cameras.end(), camera);
        if (it != m_cameras.end()) m_cameras.erase(it);
}

void Scene::registerEntity(Entity *entity) {
        m_entities.push_back(entity);
}

void Scene::unregisterEntity(Entity *entity) {
        auto it = std::find(m_entities.begin(), m_entities.end(), entity);
        if (it != m_entities.end()) m_entities.erase(it);
}

void Scene::registerVolume(Volume *volume) {
        m_volumes.push_back(volume);
}

void Scene::unregisterVolume(Volume *volume) {
        auto it = std::find(m_volumes.begin(), m_volumes.end(), volume);
        if (it != m_volumes.end()) m_volumes.erase(it);
}

void Scene::registerLight(Light *light) {
        m_lights.push_back(light);
}

void Scene::unregisterLight(Light *light) {
        auto it = std::find(m_lights.begin(), m_lights.end(), light);
        if (it != m_lights.end()) m_lights.erase(it);
}

//########################### DefaultUserScene ###############################//

DefaultUserScene::DefaultUserScene(GraphicApplication* application) :
        Scene(application),
        m_pickedObject(nullptr)
{
        m_btn = 0;
}

void DefaultUserScene::mousePressEvent(int x, int y, char button) {
        Scene::mousePressEvent(x, y, button);
        m_btn = button;
        m_pickedObject = dynamic_cast<Movable*>(pickObject(x, y));

        if (m_pickedObject) {
                m_distance = glm::length(activeCamera()->worldPosition() - m_pickedObject->worldPosition());

                Vec2 winSize = Vec2(application()->window()->size());
                Vec3 ndc = glm::project(m_pickedObject->worldPosition(), activeCamera()->worldToLocalMatrix(), activeCamera()->viewToClipMatrix(), Vec4(0,0,winSize));

                IVec2 p = application()->window()->mouse()->position();
                m_offset = p - IVec2(ndc.x, winSize.y - ndc.y);
                IM_LOG(ndc);
        }
}

void DefaultUserScene::mouseReleaseEvent(int x, int y, char button) {
        Scene::mouseReleaseEvent(x, y, button);

        m_pickedObject = nullptr;
        m_btn = 0;
}

void DefaultUserScene::mouseMoveEvent(int oldX, int oldY, int newX, int newY) {
        if (m_btn == MouseButton::RIGHT) processRotateObject(oldX, oldY, newX, newY);
        if (m_btn == MouseButton::LEFT) processMoveObject();
}

void DefaultUserScene::keyPressEvent(int key) {
        if (m_btn == MouseButton::LEFT) processMoveObject();
}

void DefaultUserScene::processMoveObject() {
        if (!m_pickedObject) return;

        float aspectRatio = activeCamera()->aspectRatio();
        float tanHalfFovy = glm::tan(glm::radians(activeCamera()->fieldOfView()/2));
        Vec2 winSize = Vec2(application()->window()->size());

        /// Приводим экранные координаты к [-1;1]
        IVec2 p = application()->window()->mouse()->position() - m_offset;
        Vec2 m = Vec2(p.x, winSize.y - p.y)/winSize;
        m = 2.0f * m - Vec2(1.0);

        Vec3 vViewRay = Vec3(
                m.x * aspectRatio * tanHalfFovy,
                m.y * tanHalfFovy,
                -1
        );
        vViewRay = glm::normalize(vViewRay);


        Vec3 viewPosition = vViewRay * m_distance;
        Vec3 worldPosition = activeCamera()->convertLocalToWorld(viewPosition);

        m_pickedObject->setWorldPosition(worldPosition);
}

void DefaultUserScene::processRotateObject(int oldX, int oldY, int newX, int newY) {
        if (!m_pickedObject) return;

        Vec2 delta = Vec2(newX, newY) - Vec2(oldX, oldY);
        Vec2 angles = delta;

        Vec3 worldSpaceCameraUp = activeCamera()->convertLocalToWorld(Vec3(0,1,0)) - activeCamera()->worldPosition();
        Vec3 worldSpaceCameraRight = activeCamera()->convertLocalToWorld(Vec3(1,0,0)) - activeCamera()->worldPosition();
        m_pickedObject->rotate(worldSpaceCameraUp, angles.x, Space::WORLD);
        m_pickedObject->rotate(worldSpaceCameraRight, angles.y, Space::WORLD);
}

} //namespace imEngine
