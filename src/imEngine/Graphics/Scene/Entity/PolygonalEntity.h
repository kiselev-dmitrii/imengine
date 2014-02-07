#ifndef POLYGONALENTITY_H
#define POLYGONALENTITY_H

#include "EntityAbstract.h"
#include "../Model.h"

namespace imEngine {

/** @brief Полигональный объект на сцене
 */
class PolygonalEntity : public EntityAbstract {
public:
        /// Конструктор
        PolygonalEntity(SceneObject* parent);

        void            render();

private:
        Model*          m_model;
};

} //namespace imEngine

#endif // POLYGONALENTITY_H
