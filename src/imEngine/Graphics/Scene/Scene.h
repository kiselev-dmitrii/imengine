#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <imEngine/Application/GraphicApplication.h>
#include "Entity/EntityAbstract.h"
#include "Light/ILight.h"
#include "Camera.h"

namespace imEngine {

typedef std::vector<LightAbstract*>     LightList;
typedef std::vector<EntityAbstract*>    EntityList;
typedef std::vector<CameraAbstract*>    CameraList;


/** @brief Класс представляет собой контейнер объектов типа
 *  камера, модель, свет и т.д
 *
 *  Сцена может быть сохранена или загружена из специального xml
 *  файла с описанием сцены.
 *  При визуализации сцены могут быть включены специальные постэффекты.
 */
class Scene {
friend class CameraAbstract;
friend class LightAbstract;
friend class EntityAbstract;
public:
        /// Конструктор
        explicit Scene(GraphicApplication* application);
        /// Деструктор
        ~Scene();

        /// Корневой объект сцены
        SceneObject*            world();
        /// Возвращает ссылку на приложение
        GraphicApplication*     application();

        /// Возвращает текущую камеру
        CameraAbstract*         currentCamera();
        /// Устанавливает камеру текущей
        void                    setCurrentCamera(CameraAbstract* camera);

        /// Обновление объектов сцены
        void                    processUpdate(float deltaTime);
        /// Рендер объектов сцены
        void                    processRender();
        /// Обработка события на изменение размеров окна
        void                    processWindowResize(int w, int h);

private:
        /// Добавляет/удаляет визуализируемый объект
        void                    addEntity(EntityAbstract* entity);
        void                    removeEntity(EntityAbstract* entity);
        /// Добавляет/удаляет камеру на сцену
        void                    addCamera(CameraAbstract* camera);
        void                    removeCamera(CameraAbstract* camera);
        /// Добавляет/удаляет источник света
        void                    addLight(LightAbstract* light);
        void                    removeLight(LightAbstract* light);

private:
        SceneObject*            m_world;
        GraphicApplication*     m_application;

        LightList               m_lights;
        CameraList              m_cameras;
        EntityList              m_enitities;

        CameraAbstract*         m_currentCamera;

};


} //namespace imEngine

#endif // SCENE_H
