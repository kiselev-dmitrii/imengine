#ifndef SLIDER_H
#define SLIDER_H

#include "TexturedWidget.h"

namespace imEngine {

class HSliderButton;
class VSliderButton;


/** @brief Горизонтальный слайдер
 */
class HSlider : public HStretchableTexturedWidget {
friend class HSliderButton;
public:
        /// Конструктор.
        /// sliderBackground, sliderSelection - задний фон слайдера и текстура выделенной области
        /// btnActive, btnHover - текстурки для кнопки
        HSlider(const String& sliderBackground, const String& sliderSelection,
                const String& btnActive, const String& btnHover, WidgetAbstract* parent);

        /// Вызывается при клике мыши на слайдер
        bool    onMousePress(int x, int y, char button);

private:
        /// Вызывается при клике изменения положения кнопки для обновления выделенной области
        void    updateSelection();

private:
        HStretchableTexturedWidget*     m_selection;
        HSliderButton*                  m_button;
};


/** @brief Вертикальный слайдер
 */
class VSlider : public VStretchableTexturedWidget {
friend class VSliderButton;
public:
        /// Конструктор.
        /// sliderBackground, sliderSelection - задний фон слайдера и текстура выделенной области
        /// btnActive, btnHover - текстурки для кнопки
        VSlider(const String& sliderBackground, const String& sliderSelection,
                const String& btnActive, const String& btnHover, WidgetAbstract* parent);

        /// Вызывается при клике мыши на слайдер
        bool    onMousePress(int x, int y, char button);

private:
        /// Вызывается при клике изменения положения кнопки для обновления выделенной области
        void    updateSelection();

private:
        VStretchableTexturedWidget*     m_selection;
        VSliderButton*                  m_button;
};


} //namesapce imEngine

#endif // SLIDER_H
