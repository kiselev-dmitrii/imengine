#ifndef SLIDER_H
#define SLIDER_H

#include "TexturedWidget.h"

namespace imEngine {

class HSlider;

/** @brief Кнопка для слайдера.
 *  Имеет только два состояния - active и hover
 */
class SliderButton : public NonStretchableTexturedWidget {
public:
        /// Конструктор
        SliderButton(const String& active, const String& hover, WidgetAbstract* parent);

        /// Обработка действий
        bool    onMouseEnter(int x, int y);
        bool    onMouseLeave(int x, int y);
        bool    onMousePress(int x, int y, char button);
        bool    onMouseRelease(int x, int y, char button);
        bool    onMouseMove(int x, int y);

private:
        String  m_activeImage;
        String  m_hoverImage;
        bool    m_wasPressed;
        Vec2    m_mouseOffset;
};


/** @brief Горизонтальный слайдер
 */
class HSlider : public HStretchableTexturedWidget {
public:
        /// Конструктор
        HSlider(const String& sliderBackground, const String& sliderSelection,
                const String& btnActive, const String& btnHover, WidgetAbstract* parent);
};


} //namesapce imEngine

#endif // SLIDER_H
