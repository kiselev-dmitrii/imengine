#ifndef ENTITYABSTRACT_H
#define ENTITYABSTRACT_H

#include "../SceneObject.h"
#include <imEngine/Graphics/GAPI/GAPI.h>

namespace imEngine {


/** @brief Абстрактный класс для всех визуализируемых и подвижных объектов на сцене
 *
 *  Каждая сущность рендерит себя по своему
 */
class EntityAbstract : public SceneObject {
public:
        /// Конструктор
        EntityAbstract();

        /// Рендерит сущность
        virtual void    render() = 0;

protected:
        ProgramPtr      m_program;

};


} //namespace imEngine

#endif // ENTITYABSTRACT_H
