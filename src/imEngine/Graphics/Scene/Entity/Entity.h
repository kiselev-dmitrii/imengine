#ifndef ENTITYABSTRACT_H
#define ENTITYABSTRACT_H

#include "../SceneObject.h"
#include <imEngine/Graphics/GAPI/GAPI.h>

namespace imEngine {


/** @brief Абстрактный класс для всех визуализируемых и подвижных объектов на сцене
 *
 *  Каждая сущность рендерит себя по своему
 */
class Entity : public SceneObject {
public:
        /// Конструктор
        explicit Entity(SceneObject* parent);
        /// Деструктор
        ~Entity();

        /// Рендерит сущность
        virtual void    render() = 0;
};


} //namespace imEngine

#endif // ENTITYABSTRACT_H
