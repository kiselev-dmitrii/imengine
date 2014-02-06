#ifndef ILIGHT_H
#define ILIGHT_H

#include "../SceneObject.h"
#include <imEngine/Graphics/GAPI/GAPI.h>

namespace imEngine {


/** @brief Абстрактный класс для света
 */
class ILight : public SceneObject {
public:
        /// Конструктор
        ILight();

        /// Устанавливает настройки источника света в программу program
        virtual void    bind(Program* program) = 0;
        /// Сбрасывает настройки
        virtual void    unbind(Program* program) = 0;

};


} //namespace imEngine

#endif // ILIGHT_H
