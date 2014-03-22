#ifndef PASS_H
#define PASS_H

#include <imEngine/Graphics/ScreenAlignedQuad.h>

namespace imEngine {

/** @brief Проход рендера
 * Получает на вход какие-либо текстуры или параметры.
 * Результатом работы является изображение в текущем фреймбуфере
 */
class Pass {
public:
        /// Конструтор. Создает программу
        Pass(const String& filename);

        /// Осуществляет проход
        void            apply() const;
        /// Возвращает программу
        Program*        program() const;

protected:
        /// Подготовительные действия: передача параметров в шейдер и т.д
        virtual void    prepare() = 0;

protected:
        Program*                        m_program;
        static ScreenAlignedQuad        s_quad;
};

} //namespace imEngine

#endif // PASS_H
