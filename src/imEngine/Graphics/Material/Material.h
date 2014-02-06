#ifndef MATERIAL_H
#define MATERIAL_H

#include <imEngine/Graphics/GAPI/GAPI.h>

namespace imEngine {

/** @brief Базовый класс для материалов
 *
 *  Имеет виртуальный метод bind, который применяет материал, на геометрию
 *  которая будет рендерится в дальнейшем. В результате вызова в установятся
 *  необходимые текстуры, в текущий шейдер передадутся переменные, а также
 *  установится необходимое состояние рендера.
 */
class IMaterial {
public:
        /// Создает материал
        IMaterial();

        /// Проверяет, содержит ли GPU программа необходимые входные аргументы, для ра
        virtual void    isSupport(Program* program) = 0;

        /// Биндит материал в программу Program
        virtual void    bind(Program* program) = 0;
        virtual void    unbind(Program* program) = 0;
};


class TextureMaterial : public IMaterial {
public:
        ///
        TextureMaterial(Texture2DPtr texture);
};


} //namespace imEngine

#endif // MATERIAL_H
