#ifndef SLIDER_H
#define SLIDER_H

#include "TexturedWidget.h"
#include "SliderButton.h"

namespace imEngine {


/** @brief Горизонтальный слайдер
 */
class HSlider : public HStretchableTexturedWidget {
public:
        /// Конструктор.
        /// background, selection - текстуры для заднего фона слайдера и фона выделенной части
        /// btnActive, btnHover - текстуры для соответствующих состояний кнопки
        HSlider(const String& sldBackground, const String& sldSelection,
                const String& btnActive, const String& btnHover, WidgetAbstract* parent);

        /// Обработка событий
        bool    onMousePress(int x, int y, char button);
        void    onSliderButtonMove();

private:
        HStretchableTexturedWidget*     m_selection;
        HSliderButton*                  m_button;
        float                           m_offset;                               // отступ от начала слайдера до активной области
};


} //namesapce imEngine

#endif // SLIDER_H
