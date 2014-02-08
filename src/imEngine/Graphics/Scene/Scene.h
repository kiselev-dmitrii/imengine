#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <imEngine/Application/GraphicApplication.h>
#include "Entity/Polygonal.h"
#include "Light/Light.h"
#include "Camera.h"

namespace imEngine {

typedef std::vector<Light*>             LightList;
typedef std::vector<Entity*>            EntityList;
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
friend class Entity;
public:
        /// Конструктор
        explicit Scene(GraphicApplication* application);
        /// Деструктор
        ~Scene();

        /// Устанавливает/возаращет захвачена ли мышь активной камерой
        void                    setMouseCaptured(bool enable)                   { m_isMouseCaptured = enable; }
        bool                    isMouseCaptured()                               { return m_isMouseCaptured; }

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
        /// Обработка событий клавиш мыши
        void                    processMousePress(int x, int y, char button);

private:
        /// Добавляет/удаляет визуализируемый объект
        void                    addEntity(Entity* entity);
        void                    removeEntity(Entity* entity);
        /// Добавляет/удаляет камеру на сцену
        void                    addCamera(CameraAbstract* camera);
        void                    removeCamera(CameraAbstract* camera);
        /// Добавляет/удаляет источник света
        void                    addLight(Light* light);
        void                    removeLight(Light* light);

private:
        SceneObject*            m_world;
        GraphicApplication*     m_application;

        LightList               m_lights;
        CameraList              m_cameras;
        EntityList              m_enitities;

        CameraAbstract*         m_currentCamera;

        bool                    m_isMouseCaptured;
};


} //namespace imEngine

#endif // SCENE_H
