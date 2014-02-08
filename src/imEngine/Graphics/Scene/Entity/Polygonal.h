#ifndef POLYGONALENTITY_H
#define POLYGONALENTITY_H

#include "Entity.h"
#include "../Model.h"

namespace imEngine {

/** @brief Полигональный объект на сцене
 */
class Polygonal : public Entity {
public:
        /// Конструкторы
        Polygonal(const Model& model, SceneObject* parent);

        /// Рендерит объект
        void    render();

private:
        Model   m_model;
};

} //namespace imEngine

#endif // POLYGONALENTITY_H
