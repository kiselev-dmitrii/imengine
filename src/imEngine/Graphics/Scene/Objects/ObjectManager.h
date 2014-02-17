#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H

#include "../Scene.h"
#include "Camera/Camera.h"
#include "Renderable/Polygonal.h"
#include "Renderable/Volume.h"
#include "Light/Light.h"
#include <list>

namespace imEngine {

typedef std::list<Camera*>      CameraList;
typedef std::list<Polygonal*>   PolygonalList;
typedef std::list<Volume*>      VolumeList;
typedef std::list<Light*>       LightList;

#define OBJECTS ObjectManager::instance()

/** @brief Менеджер объектов.
 * При создании, любой объект регистрируется в списке
 */
class ObjectManager {
public:
        /// Точка доступа к синглтону
        static ObjectManager* instance();

        /// Устанавливает/возвращает указатель на текущую сцену
        /// Должен вызываться при создании сцены
        void    setScene(Scene* scene);
        Scene*  scene();

        /// Регистрирует/убирает из списка зарегистрированных различных объекты
        void    registerCamera(Camera* camera);
        void    unregisterCamera(Camera* camera);
        void    registerPolygonal(Polygonal* polygonal);
        void    unregisterPolygonal(Polygonal* polygonal);
        void    registerVolume(Volume* volume);
        void    unregisterVolume(Volume* volume);
        void    registerLight(Light* light);
        void    unregisterLight(Light* light);

private:
        ObjectManager();
        ObjectManager(const ObjectManager&);
        ObjectManager& operator=(const ObjectManager&);

private:
        Scene*          m_scene;

        CameraList      m_cameras;
        PolygonalList   m_polygonals;
        VolumeList      m_volumes;
        LightList       m_lights;
};


} //namespace imEngine

#endif // OBJECTMANAGER_H
