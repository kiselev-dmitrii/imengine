#ifndef MODEL_H
#define MODEL_H

#include "../Object.h"
#include <imEngine/Graphics/Geometry.h>
#include <imEngine/Graphics/Scene/Materials/Material.h>

namespace imEngine {

class Model;


/** @brief Составная часть полигональной модели.
 */
struct ModelDetail {
        Geometry*       geometry;
        MaterialPtr     material;
        Model*          owner;
};
typedef std::vector<ModelDetail> ModelDetailList;


/** @brief Представляет набор деталей меша, отображаемых на экране
 *
 * Моделью может владеть любой объект на сцене и отображаться с ней
 */
class Model {
public:
        /// Конструктор. Создает пустую модель
        explicit Model(Object* owner);
        /// Конструктор. Создает модель с деталями details
        explicit Model(const ModelDetailList& details, Object* owner);

        /// Возвращает детали модели
        ModelDetailList&        details();
        /// Устанавливает/возвращает владельца модели
        Object*                 owner() const;

        /// Загружает модель из файла/ноды
        void                    loadFromXML(const String& filename);
        void                    loadFromXML(const XmlNode& modelNode);

private:
        Geometry*               createGeometry(const XmlNode& geometryNode);
        MaterialPtr             createMaterial(const XmlNode& materialNode);

private:
        ModelDetailList m_details;
        Object*         m_owner;
};



} //namespace imEngine

#endif // MODEL_H
