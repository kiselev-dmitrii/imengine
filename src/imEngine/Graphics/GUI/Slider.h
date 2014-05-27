#ifndef SLIDER_H
#define SLIDER_H

#include "TexturedWidget.h"
#include "SliderButton.h"

namespace imEngine {


/** @brief Абстрактный слайдер
 */
class SliderAbstract {
public:
        /// Конструктор
        SliderAbstract();

        /// Устанавливает/возвращает минимальное значение
        void            setMinValue(float value)                                { m_minValue = value; }
        float           minValue() const                                        { return m_minValue; }
        /// Устанавливает/возвращает максимальное значение
        void            setMaxValue(float value)                                { m_maxValue = value; }
        float           maxValue() const                                        { return m_maxValue; }

        /// Устанавливает минимальное и максимальное значения
        void            setMinMaxValues(float min, float max);

        /// Возвращает/устанавливает текущее значение слайдера
        void            setValue(float value);
        float           value() const;

        /// Устанавливает/возвращает текущее значение слайдера в процентах
        virtual void    setPercent(float percent) = 0;
        virtual float   percent() const = 0;

private:
        float           m_minValue;
        float           m_maxValue;
};


/** @brief Горизонтальный слайдер
 */
class HSlider : public HStretchableTexturedWidget, public SliderAbstract {
public:
        /// Конструктор.
        /// background, selection - текстуры для заднего фона слайдера и фона выделенной части
        /// btnActive, btnHover - текстуры для соответствующих состояний кнопки
        HSlider(const String& sldBackground, const String& sldSelection,
                const String& btnActive, const String& btnHover, WidgetAbstract* parent);

        /// Загружает настройки из XML-ноды
        void    loadFromXml(const XmlNode &node);

        /// Устанавливает/возвращает значение слайдера в процентах
        void    setPercent(float percent);
        float   percent() const;

        /// Обработка событий
        bool    onMousePress(int x, int y, char button);
        void    onSliderButtonMove();

public:
        /// Сигнал вызывается при изменении положения бегунка
        Signal<HSlider*>        onValueChanged;

private:
        HStretchableTexturedWidget*     m_selection;
        HSliderButton*                  m_button;
        float                           m_offset;                               // отступ от начала слайдера до активной области
};


/** @brief Вертикальный слайдер
 */
class VSlider : public VStretchableTexturedWidget, public SliderAbstract {
public:
        /// Конструктор.
        /// background, selection - текстуры для заднего фона слайдера и фона выделенной части
        /// btnActive, btnHover - текстуры для соответствующих состояний кнопки
        VSlider(const String& sldBackground, const String& sldSelection,
                const String& btnActive, const String& btnHover, WidgetAbstract* parent);

        /// Загружает настройки из XML-ноды
        void    loadFromXml(const XmlNode &node);

        /// Устанавливает/возвращает значение слайдера в процентах
        void    setPercent(float percent);
        float   percent() const;

        /// Обработка событий
        bool    onMousePress(int x, int y, char button);
        void    onSliderButtonMove();
        void    onHeightChange();

public:
        /// Сигнал вызывается при изменении положения бегунка
        Signal<VSlider*>        onValueChanged;

private:
        VStretchableTexturedWidget*     m_selection;
        VSliderButton*                  m_button;
        float                           m_offset;
};


} //namesapce imEngine

#endif // SLIDER_H
