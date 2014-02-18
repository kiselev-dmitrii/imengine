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

        /// Возвращает модель
        Model&          model();

private:
        Model           m_model;
};


} //namespace imEngine

#endif // POLYGONALENTITY_H
