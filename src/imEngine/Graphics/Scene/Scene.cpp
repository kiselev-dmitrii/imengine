#include "Scene.h"
#include <algorithm>
#include <imEngine/Utils/Debug.h>

namespace imEngine {

/** @brief Представляет собой кореневой объект сцены
 */
class SceneWorld : public SceneObject {
public:
        /// Конструктор
        SceneWorld(Scene* scene) : SceneObject(nullptr)                         { m_scene = scene; }

};

//############################## Scene #######################################//

Scene::Scene(GraphicApplication *application) :
        m_application(application)
{
        m_world = new SceneWorld(this);
        m_currentCamera = new FirstPersonCamera(m_world);
}

Scene::~Scene() {
        delete m_world;
}

void Scene::processUpdate(float deltaTime) {
        m_currentCamera->update(deltaTime);
}

void Scene::processRender() {
        for(CameraAbstract* camera: m_cameras) {
                if (camera != m_currentCamera) camera->renderHelper();
        }
        for(Entity* entity: m_enitities) {
                entity->render();
        }
}

SceneObject* Scene::world() {
        return m_world;
}

GraphicApplication* Scene::application() {
        return m_application;
}

CameraAbstract* Scene::currentCamera() {
        return m_currentCamera;
}

void Scene::setCurrentCamera(CameraAbstract *camera) {
        m_currentCamera = camera;
}

void Scene::processWindowResize(int w, int h) {
        m_currentCamera->setAspectRatio(float(w)/h);
}

void Scene::addEntity(Entity *entity) {
        m_enitities.push_back(entity);
}

void Scene::removeEntity(Entity *entity) {
        auto it = std::find(m_enitities.begin(), m_enitities.end(), entity);
        m_enitities.erase(it);
}

void Scene::addCamera(CameraAbstract *camera) {
        m_cameras.push_back(camera);
}

void Scene::removeCamera(CameraAbstract *camera) {
        auto it = std::find(m_cameras.begin(), m_cameras.end(), camera);
        m_cameras.erase(it);
}

void Scene::addLight(Light* light) {
        m_lights.push_back(light);
}

void Scene::removeLight(Light* light) {
        auto it = std::find(m_lights.begin(), m_lights.end(), light);
        m_lights.erase(it);
}


} //namespace imEngine
