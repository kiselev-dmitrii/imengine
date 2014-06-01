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
        /// Деструктор
        ~Entity();

        /// Загружает настройки из json-ноды
        void            loadFromJson(const JsonValue &node);
        /// Сохраняет настройки в виде json-ноды
        void            saveAsJson(JsonValue &result);

        /// Возвращает модель
        Model*          model()                                                 { return &m_model; }

protected:
        Model   m_model;
};


} //namespace imEngine


#endif // ENTITY_H
