#ifndef ENTITY_H
#define ENTITY_H

#include "../Movable.h"
#include "Model.h"

namespace imEngine {


/** @brief Трехмерный объект состоящий из множества полигонов
 */
class Entity : public Movable {
public:
        /// Конструктор. Создает сущеность с моделью из файла model
        Entity(const String& model, Object* parent);
        /// Конструктор. Создает полигональный объект из одной детали
        Entity(const String& geometry, EntityMaterialPtr material, Object* parent);
        /// Деструктор
        ~Entity();

        /// Возвращает модель
        Model*          model()                                                 { return &m_model; }

protected:
        Model   m_model;
};


} //namespace imEngine


#endif // ENTITY_H