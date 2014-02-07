#ifndef POLYGONALENTITY_H
#define POLYGONALENTITY_H

#include "EntityAbstract.h"
#include "../Geometry.h"
#include "../Material/Material.h"

namespace imEngine {

/** @brief Полигональный объект на сцене
 */
class PolygonalEntity : public EntityAbstract {
public:
        /// Конструктор
        PolygonalEntity(SceneObject* parent);

        void            render();

private:
        GeometryPtr     m_geometry;
        WiredMaterial*  m_material;
};

} //namespace imEngine

#endif // POLYGONALENTITY_H
