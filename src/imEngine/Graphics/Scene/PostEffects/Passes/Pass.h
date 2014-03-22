#ifndef PASS_H
#define PASS_H

#include <imEngine/Graphics/GAPI/GAPI.h>

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
        virtual void    prepare() const = 0;

protected:
        Program*        m_program;
};


/** @brief Эквивалентный проход. Просто рендерит текстуру
 */
class BlitCopyPass : public Pass {
public:
        /// Конструктор
        BlitCopyPass(Texture2D* texture);
        /// Устанавливает текстуру
        void    setTexture(Texture2D* texture)                                  { m_texture = texture; }

protected:
        void    prepare() const;

private:
        Texture2D*      m_texture;
};


/** @brief Проход, в результате которго остаются пиксели с яркостью больше threshold
 */
class ThresholdPass : public Pass {
public:
        /// Конструктор
        ThresholdPass(Texture2D* texture);
        /// Устанавливает текстуру
        void    setTexture(Texture2D* texture)                                  { m_texture = texture; }
        /// Устанавливает порог
        void    setThreshold(float threshold)                                   { m_threshold = threshold; }

protected:
        void    prepare() const;

private:
        Texture2D*      m_texture;
        float           m_threshold;
};


} //namespace imEngine

#endif // PASS_H
