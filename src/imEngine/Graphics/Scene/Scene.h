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

        /*
         * Model*       models(const String& name);
         * Material*    materials(consdt String& name);
private:
        typedef std::vector<ObjectPtr>  ObjectList;
        typedef std::vector<CameraPtr>  CameraList;
        typedef std::vector<LightPtr>   LightList;
        typedef std::vector<ModelPtr>   ModelList
        */

};

} //namespace imEngine

#endif // SCENE_H
