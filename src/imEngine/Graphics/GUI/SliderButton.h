#ifndef SLIDERBUTTON_H
#define SLIDERBUTTON_H

#include "Button.h"

namespace imEngine {


/** @brief Абстрактная кнопка для слайдера
 */
class SliderButtonAbstract : public Button {
public:
        /// Конструктор
        /// active, hover - картинки с соответсвующими состояниями
        /// start, end - смещения с конца и с начала, где будет двигаться кнопка
        SliderButtonAbstract(const String& active, const String& hover, float startOffset, float endOffset, WidgetAbstract* parent);

        /// Обработка нажатия на кнопку для слайдера
        bool            onMousePress(int x, int y, char button);
        /// Обработка движения слайдера
        void            onGlobalMouseMove(int x, int y) = 0;

        /// Установка новых точек смещения
        void            setOffsets(float start, float end);
        /// Возвращает точки смещения
        float           startOffset() const;
        float           endOffset() const;

        /// Установка положения слайдера в процентах.
        virtual void    setPercent(float percent) = 0;
        /// Возвращает положение слайдера в процентах
        float           percent() const;

        /// Установка валидного положения слайдера
        virtual void    setValidPosition(const Vec2& position) = 0;

protected:
        Vec2    m_mousePosition;                //позиция мыши на момент нажатия клавиши мыши
        float   m_startOffset;
        float   m_endOffset;
        float   m_percent;
};


/** @brief Кнопка для горизонтального слайдера
 */
class HSliderButton : public SliderButtonAbstract {
public:
        /// Конструктор
        HSliderButton(const String& active, const String& hover, float startOffset, float endOffset, WidgetAbstract* parent);

        /// Перегруженные методы
        void            onGlobalMouseMove(int x, int y);
        void            setPercent(float percent);
        void            setValidPosition(const Vec2& position);
};


/** @brief Кнопка для вертикального слайдера
 */
class VSliderButton : public SliderButtonAbstract {
public:
        /// Конструктор
        VSliderButton(const String& active, const String& hover, float startOffset, float endOffset, WidgetAbstract* parent);

        /// Перегруженные методы
        void            onGlobalMouseMove(int x, int y);
        void            setPercent(float percent);
        void            setValidPosition(const Vec2& position);
};


} // namespace imEngine

#endif // SLIDERBUTTON_H
