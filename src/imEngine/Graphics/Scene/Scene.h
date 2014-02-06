#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <imEngine/Application/GraphicApplication.h>
#include "Entity/EntityAbstract.h"
#include "Light/ILight.h"
#include "Camera.h"

namespace imEngine {

typedef std::vector<ILight>             LightList;
typedef std::vector<EntityAbstract>     EntityList;
typedef std::vector<CameraAbstract>     CameraList;

/** @brief Класс представляет собой контейнер объектов типа
 *  камера, модель, свет и т.д
 *
 *  Сцена может быть сохранена или загружена из специального xml
 *  файла с описанием сцены.
 *  При визуализации сцены могут быть включены специальные постэффекты.
 */
class Scene {
public:
        /// Конструктор
        explicit Scene(GraphicApplication* application);
        /// Деструктор
        ~Scene();

        /// Инициализация сцены
        void            initialize();
        /// Обновление сцены
        void            update(float deltaTime);
        /// Рендер сцены
        void            render();
        /// Разрушение объектов сцены
        void            destroy();

private:
        SceneObject*    m_root;
        LightList       m_lights;
        CameraList      m_cameras;
        EntityList      m_enitities;
};

} //namespace imEngine

#endif // SCENE_H
