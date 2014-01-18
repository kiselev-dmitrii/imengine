#include "SliderButton.h"
#include <imEngine/Utils/Debug.h>
#include "Slider.h"

namespace imEngine {


//########################## SliderButtonAbstract ############################//

SliderButtonAbstract::SliderButtonAbstract(const String &active, const String &hover, float startOffset, float endOffset, WidgetAbstract *parent) :
        Button(active, hover, hover, active, active, parent),
        m_mousePosition(0, 0),
        m_startOffset(startOffset),
        m_endOffset(endOffset),
        m_percent(0)
{  }

bool SliderButtonAbstract::onMousePress(int x, int y, char button) {
        if (button != MouseButton::LEFT) return false;
        Button::onMousePress(x, y, button);
        m_mousePosition = absoluteToLocal(Vec2(x,y));
        return true;
}

void SliderButtonAbstract::setOffsets(float start, float end) {
        m_startOffset = start;
        m_endOffset = end;
}

float SliderButtonAbstract::startOffset() const {
        return m_startOffset;
}

float SliderButtonAbstract::percent() const {
        return m_percent;
}

//############################# HSliderButton ################################//

HSliderButton::HSliderButton(const String &active, const String &hover, float startOffset, float endOffset, WidgetAbstract *parent) :
        SliderButtonAbstract(active, hover, startOffset, endOffset, parent)
{ }

void HSliderButton::onGlobalMouseMove(int x, int y) {
        if (!isPressed()) return;

        float posx = left();
        float start = m_startOffset;
        float end = ((WidgetAbstract*)parent())->width() - m_endOffset - width();
        if (posx >= start && posx <= end) {
                // Обновляем позицию
                float delta = (absoluteToLocal(Vec2(x,y)) - m_mousePosition).x;
                posx = glm::clamp(posx + delta, start, end);
                setLeft(posx);

                // Обновляем позицию в процентах
                m_percent = (posx - start) / (end - start);

                // Сообщаем родителю, что позиция слайдера изменилась
                ((HSlider*)parent())->onSliderButtonMove();
        }
}

void HSliderButton::setPercent(float percent) {
        m_percent = glm::clamp(percent, 0.0f, 1.0f);

        // Устанавливаем позицию исходя из процентов
        float start = m_startOffset;
        float end = ((WidgetAbstract*)parent())->width() - m_endOffset - width();
        setLeft(start + m_percent*(end-start));

        // Сообщаем родителю, что позиция слайдера изменилась
        ((HSlider*)parent())->onSliderButtonMove();
}

void HSliderButton::setValidPosition(const Vec2 &position) {
        float start = m_startOffset;
        float end = ((WidgetAbstract*)parent())->width() - m_endOffset - width();
        setLeft(glm::clamp(position.x - width()/2, start, end));

        // Сообщаем родителю, что позиция слайдера изменилась
        ((HSlider*)parent())->onSliderButtonMove();
}


} //namespace imEngine
