#ifndef ILIGHT_H
#define ILIGHT_H

#include "../Movable.h"
#include <imEngine/Graphics/GAPI/GAPI.h>

namespace imEngine {


/** @brief Абстрактный класс для света
 */
class Light : public Movable {
public:
        /// Конструктор
        Light(Object* parent);

        /// Устанавливает настройки источника света в программу program
        virtual void    bind() = 0;
        /// Сбрасывает настройки
        virtual void    unbind() = 0;
};


} //namespace imEngine

#endif // ILIGHT_H
