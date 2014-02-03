#ifndef MODEL_H
#define MODEL_H

#include "Material.h"
#include "Geometry.h"

namespace imEngine {


/** @brief Составная часть полигональной модели.
 *
 * Содержит геометрию в виде VBO, IBO, VAO, а также материал с которым будет
 * производиться визуализация.
 * Также модель имеет шейдер, посредством которого происходит визуализация
 */
class ModelDetail {
public:
private:
        Geometry*       m_geometry;
        Material*       m_material;
};


/** @brief Представляет набор деталей меша, отображаемых на экране
 */
class Model {
public:
        Model();

private:
        typedef std::vector<ModelDetail> ModelDetailList;
        ModelDetailList m_details;
};


} //namespace imEngine

#endif // MODEL_H
