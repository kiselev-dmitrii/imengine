#ifndef SLIDERBUTTON_H
#define SLIDERBUTTON_H

#include "Button.h"

namespace imEngine {


/** @brief Кнопка, подвижная только в одном измерении
 */
class SliderButton : public Button {
public:
        /// Конструктор.
        /// active, hover - картинки с соответствующим состоянием
        /// start, end - точки в родительской системе координат, между которыми будет двигаться кнопка
        SliderButton(const String& active, const String& hover,  const Vec2& start, const Vec2& end, WidgetAbstract* parent);

        /// Обработка нажатия на кнопку для слайдера
        bool    onMousePress(int x, int y, char button);
        /// Обработка движения слайдера
        void    onGlobalMouseMove(int x, int y);

        /// Установка конечной и начальной точки
        void    setBounds(const Vec2& start, const Vec2& end);
        /// Получение конечной и начальной точки
        Vec2    startBound() const;
        Vec2    endBound() const;

        /// Установка и получение положения слайдера в процентах
        float   percent() const;
        void    setPercent(float percent);

private:
        Vec2    m_prevMousePos;
        Vec2    m_start;
        Vec2    m_end;
        float   m_percent;
};


} // namespace imEngine

#endif // SLIDERBUTTON_H
