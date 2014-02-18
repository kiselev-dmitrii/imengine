#include "Scene.h"
#include "Objects/Camera/FirstPersonCamera.h"

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
}

Scene::~Scene() {
        delete m_world;
}

Object* Scene::world() {
        return m_world;
}

void Scene::setActiveCamera(Camera *camera) {
        m_activeCamera = camera;
}

Camera* Scene::activeCamera() const {
        return m_activeCamera;
}

void Scene::setInputCaptured(bool enable) {
        m_isInputCaptured = enable;
}

bool Scene::isInputCaptured() const {
        return m_isInputCaptured;
}

GraphicApplication* Scene::application() {
        return m_application;
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

void Scene::windowResizeEvent(int w, int h) {
        for (Camera* camera: m_cameras) camera->setAspectRatio(float(w)/h);
        m_activeCamera->setAspectRatio(float(w)/h);
}

void Scene::registerCamera(Camera *camera) {
        m_cameras.push_back(camera);
}

void Scene::unregisterCamera(Camera *camera) {
        auto it = std::find(m_cameras.begin(), m_cameras.end(), camera);
        if (it != m_cameras.end()) m_cameras.erase(it);
}

void Scene::registerPolygonal(Polygonal *polygonal) {
        m_polygonals.push_back(polygonal);
}

void Scene::unregisterPolygonal(Polygonal *polygonal) {
        auto it = std::find(m_polygonals.begin(), m_polygonals.end(), polygonal);
        if (it != m_polygonals.end()) m_polygonals.erase(it);
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

} //namespace imEngine
