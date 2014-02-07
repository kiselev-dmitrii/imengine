#ifndef MODEL_H
#define MODEL_H

#include "Material/Material.h"
#include "Geometry.h"

namespace imEngine {


/** @brief Составная часть полигональной модели.
 *
 *  Содержит геометрию и матриал, с которым будет производится визуализация
 */
class ModelDetail {
public:
        /// Конструктор.
        ModelDetail(Geometry* geometry, Material* material);

        /// Рендерит деталь с выбранным материалом
        void            render();

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
