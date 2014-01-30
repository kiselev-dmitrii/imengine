#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "Object.h"

namespace imEngine {

/** @brief Класс представляет собой контейнер объектов типа
 *  камера, модель, свет и т.д
 *
 *  Сцена может быть сохранена или загружена из специального xml
 *  файла с описанием сцены.
 */
class Scene {
public:
        Scene();

private:
        /*
        typedef std::vector<ObjectPtr>  ObjectList;
        typedef std::vector<CameraPtr>  CameraList;
        typedef std::vector<LightPtr>   LightList;
        */
};

} //namespace imEngine

#endif // SCENE_H
