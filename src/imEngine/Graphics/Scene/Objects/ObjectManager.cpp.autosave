#include "ObjectManager.h"
#include <algorithm>

namespace imEngine {

ObjectManager::ObjectManager() :
        m_scene(nullptr)
{ }

ObjectManager* ObjectManager::instance() {
        static ObjectManager instance;
        return &instance;
}

void ObjectManager::setScene(Scene *scene) {
        m_scene = scene;
}

Scene* ObjectManager::scene() {
        return m_scene;
}

void ObjectManager::registerCamera(Camera *camera) {
        m_cameras.push_back(camera);
}

void ObjectManager::unregisterCamera(Camera *camera) {
        auto it = std::find(m_cameras.begin(), m_cameras.end(), camera);
        if (it != m_cameras.end()) m_cameras.erase(it);
}

void ObjectManager::registerPolygonal(Polygonal *polygonal) {
        m_polygonals.push_back(polygonal);
}

void ObjectManager::unregisterPolygonal(Polygonal *polygonal) {
        auto it = std::find(m_polygonals.begin(), m_polygonals.end(), polygonal);
        if (it != m_polygonals.end()) m_polygonals.erase(it);
}

void ObjectManager::registerVolume(Volume *volume) {
        m_volumes.push_back(volume);
}

void ObjectManager::unregisterVolume(Volume *volume) {
        auto it = std::find(m_volumes.begin(), m_volumes.end(), volume);
        if (it != m_volumes.end()) m_volumes.erase(it);
}

void ObjectManager::registerLight(Light *light) {
        m_lights.push_back(light);
}

void ObjectManager::unregisterLight(Light *light) {
        auto it = std::find(m_lights.begin(), m_lights.end(), light);
        if (it != m_lights.end()) m_lights.erase(it);
}

}
