#ifndef MODEL_H
#define MODEL_H

#include "Material/Material.h"
#include "Scene.h"
#include "Geometry.h"

namespace imEngine {

/** @brief Составная часть полигональной модели.
 */
struct ModelDetail {
        GeometryPtr     geometry;
        MaterialPtr     material;
};
typedef std::vector<ModelDetail> ModelDetailList;

/** @brief Представляет набор деталей меша, отображаемых на экране
 */
class Model {
public:
        /// Конструктор. Загружает модель
        explicit Model(const String& filename);

        /// Возвращает детали модели
        const ModelDetailList&  details() const;

        /// Рендерит модель из камеры camera на позиции объекта object
        void                    render(CameraAbstract* camera, SceneObject* object);

private:
        ModelDetailList m_details;
};


} //namespace imEngine

#endif // MODEL_H
