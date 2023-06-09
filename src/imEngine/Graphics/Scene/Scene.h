#ifndef SCENE_H
#define SCENE_H

#include <imEngine/Application/GraphicApplication.h>
#include "Objects/Camera/Camera.h"
#include "Objects/Renderable/Entity.h"
#include "Objects/Renderable/Volume.h"
#include "Objects/Light/Light.h"
#include <list>

namespace imEngine {

typedef std::list<Camera*>      CameraList;
typedef std::list<Entity*>      EntityList;
typedef std::list<Volume*>      VolumeList;
typedef std::list<Light*>       LightList;
class SceneRenderer;

/** @brief Базовая сцена
 *
 * Занимается менеджментом объектов
 */
class Scene {
public:
        /// Конструктор
        explicit Scene(GraphicApplication* application);
        /// Деструктор
        virtual ~Scene();

        /// Загружает сцену из файла
        void            loadScene(const String& filename);
        /// Сохраняет сцену в файл
        void            saveScene(const String& filename);

        /// Возвращает корневой объект сцены
        Object*         world()                                                 { return m_world; }

        /// Устанавливает/возвращает активную камеру
        void            setActiveCamera(Camera* camera)                         { m_activeCamera = camera; }
        Camera*         activeCamera() const                                    { return m_activeCamera; }

        /// Захватывает ввод сценой
        void            setInputCaptured(bool enable)                           { m_isInputCaptured = enable; }
        bool            isInputCaptured() const                                 { return m_isInputCaptured; }

        /// Возвращает указатель на рендер сцены
        SceneRenderer*  renderer()                                              { return m_sceneRenderer; }
        /// Возвращает указатель на приложение
        GraphicApplication* application()                                       { return m_application; }

        /// Возвращает объект с экранными координатами (x,y)
        Object*         pickObject(int x, int y);

        /// Рендерит сцену
        void            render();
        /// Вызывается при изменении размеров окна
        void            windowResizeEvent(int w, int h);

public:
        /// Переопределяются в пользовательских сценах
        virtual void    update(float delta);
        virtual void    mousePressEvent(int x, int y, char button);
        virtual void    mouseReleaseEvent(int x, int y, char button)            { }
        virtual void    mouseMoveEvent(int oldX, int oldY, int newX, int newY)  { }
        virtual void    keyPressEvent(int key)                                  { }
        virtual void    keyReleaseEvent(int key)                                { }

public:
        /// Регистрирует/убирает из списка зарегистрированных различных объекты
        void            registerCamera(Camera* camera);
        void            unregisterCamera(Camera* camera);
        void            registerEntity(Entity* entity);
        void            unregisterEntity(Entity* entity);
        void            registerVolume(Volume* volume);
        void            unregisterVolume(Volume* volume);
        void            registerLight(Light* light);
        void            unregisterLight(Light* light);

        /// Возвращает список доступных обхектов
        CameraList&     cameras()                                               { return m_cameras; }
        EntityList&     entities()                                              { return m_entities; }
        VolumeList&     volumes()                                               { return m_volumes; }
        LightList&      ligths()                                                { return m_lights; }

protected:
        GraphicApplication*     m_application;
        SceneRenderer*          m_sceneRenderer;

        Object*         m_world;
        Camera*         m_activeCamera;
        bool            m_isInputCaptured;

        CameraList      m_cameras;
        EntityList      m_entities;
        VolumeList      m_volumes;
        LightList       m_lights;
};


/** @brief Стандартная сцена
 */
class DefaultUserScene : public Scene {
public:
        /// Конструктор
        DefaultUserScene(GraphicApplication* application);

        /// Переопределены для управления сценой
        void    mousePressEvent(int x, int y, char button);
        void    mouseReleaseEvent(int x, int y, char button);
        void    mouseMoveEvent(int oldX, int oldY, int newX, int newY);
        void    keyPressEvent(int key);

private:
        void    processMoveObject();
        void    processRotateObject(int oldX, int oldY, int newX, int newY);

private:
        Movable*                m_pickedObject;
        char                    m_btn;
        float                   m_distance;
        IVec2                   m_offset;
};

} //namespace imEngine

#endif // SCENE_H
