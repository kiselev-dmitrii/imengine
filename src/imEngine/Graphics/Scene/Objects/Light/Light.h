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
        Light(const String& filename, Object* parent);

        /// Устанавливает настройки света перед рендером для сцены scene
        virtual void    bind(Scene* scene) = 0;
        /// Сбрасывает настройки
        virtual void    unbind() = 0;

        /// Возвращает программу
        Program*        program() const;

protected:
        Program*        m_program;
};


} //namespace imEngine

#endif // ILIGHT_H
