#ifndef SLIDER_H
#define SLIDER_H

#include "TexturedWidget.h"
#include "Button.h"
#include "Signal.h"

namespace imEngine {

class HSlider;

/** @brief Кнопка для слайдера.
 *  Имеет только два состояния - active и hover
 */
class SliderButton : public Button {
public:
        /// Конструктор.
        /// active, hover - состяния бегунка.
        /// bound - расстояние от краев родительского виджета
        SliderButton(const String& active, const String& hover, float bound, WidgetAbstract* parent);

        /// Возвращает положение слайдера в процентах
        float   percent() const                                                 { return m_percent; }

        /// Обработка действий
        bool    onMousePress(int x, int y, char button);
        void    onGlobalMouseMove(int x, int y);

        /// Устанавливает позицию бегунка если она является допустимой
        void    setOffset(float x);

public:
        Signal<float>   onValueChanged;

private:
        Vec2    m_mousePosPrev;
        float   m_bound;
        float   m_percent;
};


/** @brief Горизонтальный слайдер
 */
class HSlider : public HStretchableTexturedWidget {
public:
        /// Конструктор
        HSlider(const String& sliderBackground, const String& sliderSelection,
                const String& btnActive, const String& btnHover, WidgetAbstract* parent);

        /// При клике мыши - переставляем туда курсор
        bool    onMousePress(int x, int y, char button);

private:
        void    resizeSelection();

private:
        HStretchableTexturedWidget*     m_selection;
        SliderButton*                   m_button;
};


} //namesapce imEngine

#endif // SLIDER_H
