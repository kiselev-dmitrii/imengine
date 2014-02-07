#ifndef MODEL_H
#define MODEL_H

#include "Material/Material.h"
#include "Scene.h"
#include "Geometry.h"
#include <imEngine/FileContainers/Xml.h>

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

        /// Загружает модель из файла
        void    load(const String& filename);
        /// Загружает модель из ноды
        void    load(const XmlNode& modelNode);

        /// Рендерит модель из камеры camera на позиции объекта object
        void    render(CameraAbstract* camera, SceneObject* object);

        /// Возвращает детали модели
        const ModelDetailList&  details() const;

private:
        GeometryPtr     createGeometry(const XmlNode& geometryNode);
        MaterialPtr     createMaterial(const XmlNode& materialNode);
        Vec4            parseColor(const String& color);

private:
        ModelDetailList m_details;
};


} //namespace imEngine

#endif // MODEL_H
