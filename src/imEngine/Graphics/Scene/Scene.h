#ifndef SCENE_H
#define SCENE_H

#include <imEngine/Application/GraphicApplication.h>
#include "Objects/Camera/Camera.h"
#include "Objects/Renderable/Polygonal.h"
#include "Objects/Renderable/Volume.h"
#include "Objects/Light/Light.h"
#include <list>

namespace imEngine {


typedef std::list<Camera*>      CameraList;
typedef std::list<Polygonal*>   PolygonalList;
typedef std::list<Volume*>      VolumeList;
typedef std::list<Light*>       LightList;


/** @brief Базовая сцена
 */
class Scene {
public:
        /// Конструктор
        explicit Scene(GraphicApplication* application);
        /// Деструктор
        ~Scene();

        /// Возвращает корневой объект сцены
        Object*         world();

        /// Устанавливает/возвращает активную камеру
        void            setActiveCamera(Camera* camera);
        Camera*         activeCamera() const;

        /// Захватывает ввод сценой
        void            setInputCaptured(bool enable);
        bool            isInputCaptured() const;

        /// Возвращает указатель на приложение
        GraphicApplication* application();

public:
        /// Методы вызываются из GraphicApplication
        virtual void    render()                                                { }
        virtual void    update(float delta);
        virtual void    mousePressEvent(int x, int y, char button);
        virtual void    windowResizeEvent(int w, int h);

public:
        /// Регистрирует/убирает из списка зарегистрированных различных объекты
        void            registerCamera(Camera* camera);
        void            unregisterCamera(Camera* camera);
        void            registerPolygonal(Polygonal* polygonal);
        void            unregisterPolygonal(Polygonal* polygonal);
        void            registerVolume(Volume* volume);
        void            unregisterVolume(Volume* volume);
        void            registerLight(Light* light);
        void            unregisterLight(Light* light);

private:
        GraphicApplication* m_application;

        Object*         m_world;
        Camera*         m_activeCamera;
        bool            m_isInputCaptured;

        CameraList      m_cameras;
        PolygonalList   m_polygonals;
        VolumeList      m_volumes;
        LightList       m_lights;
};


} //namespace imEngine

#endif // SCENE_H
