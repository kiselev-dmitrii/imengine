#ifndef POLYGONALENTITY_H
#define POLYGONALENTITY_H

#include "../Movable.h"
#include "Model.h"

namespace imEngine {

/** @brief Полигональный объект на сцене
 */
class Polygonal : public Movable {
public:
        /// Конструкторы
        Polygonal(const String& model, Object* parent);

        /// Возвращает AABB
        const AABB&     aabb() const;
        /// Возвращает модель
        Model&          model();

private:
        /// Считает AABB исходя из делателей модели
        void            calculateAABB();

private:
        Model           m_model;
        AABB            m_aabb;
};


} //namespace imEngine

#endif // POLYGONALENTITY_H
