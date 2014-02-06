#ifndef POLYGONALENTITY_H
#define POLYGONALENTITY_H

#include "EntityAbstract.h"
#include <imEngine/Graphics/Geometry.h>

namespace imEngine {

/** @brief Полигональный объект на сцене
 */
class PolygonalEntity : public EntityAbstract {
public:
        /// Конструктор
        PolygonalEntity(SceneObject* parent);

        void            render();

private:
        ProgramPtr      m_program;
        GeometryPtr     m_geometry;
};

} //namespace imEngine

#endif // POLYGONALENTITY_H
