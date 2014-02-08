#include "Slider.h"
#include "Button.h"
#include <imEngine/Utils/Debug.h>

namespace imEngine {

//############################ SliderAbstract ################################//

SliderAbstract::SliderAbstract() :
        m_minValue(0.0),
        m_maxValue(1.0)
{}

void SliderAbstract::setMinMaxValues(float min, float max) {
        m_minValue = min;
        m_maxValue = max;
}

void SliderAbstract::setValue(float value) {
        setPercent((value - m_minValue)/(m_maxValue - m_minValue));
}

float SliderAbstract::value() const {
        return m_minValue + percent() * (m_maxValue - m_minValue);
}

//################################ HSlider ###################################//


HSlider::HSlider(const String &sldBackground, const String &sldSelection, const String &btnActive, const String &btnHover, WidgetAbstract *parent) :
        HStretchableTexturedWidget(sldBackground, parent),
        m_offset(minimalWidth()/2)
{
        // Создаем выделенную область
        m_selection = new HStretchableTexturedWidget(sldSelection, this);
        m_selection->setLeft(m_offset);

        // Создаем кнопку после выделения, чтобы она рендерилась позже
        m_button = new HSliderButton(btnActive, btnHover, m_offset, m_offset, this);
        m_button->setPercent(0);
        m_button->alignVertical(WidgetVAlignment::CENTER);
}

void HSlider::setPercent(float percent) {
        m_button->setPercent(percent);
}

float HSlider::percent() const {
        return m_button->percent();
}

bool HSlider::onMousePress(int x, int y, char button) {
        m_button->setValidPosition(absoluteToLocal(Vec2(x,y)));
        m_button->onMousePress(x, y, MouseButton::LEFT);
        return true;
}

void HSlider::onSliderButtonMove() {
        m_selection->setWidth(m_button->left() - m_button->width()/2);
        onValueChanged(this);
}

//################################ VSlider ###################################//

VSlider::VSlider(const String &sldBackground, const String &sldSelection, const String &btnActive, const String &btnHover, WidgetAbstract *parent) :
        VStretchableTexturedWidget(sldBackground, parent),
        m_offset(minimalHeight()/2)
{
        // Создаем выделенную область
        m_selection = new VStretchableTexturedWidget(sldSelection, this);

        // Создаем кнопку
        m_button = new VSliderButton(btnActive, btnHover, m_offset, m_offset, this);
        m_button->setPercent(0);
        m_button->alignHorizontal(WidgetHAlignment::CENTER);
}

void VSlider::setPercent(float percent) {
        m_button->setPercent(percent);
}

float VSlider::percent() const {
        return m_button->percent();
}

bool VSlider::onMousePress(int x, int y, char button) {
        m_button->setValidPosition(absoluteToLocal(Vec2(x,y)));
        m_button->onMousePress(x, y, MouseButton::LEFT);
        return true;
}

void VSlider::onSliderButtonMove() {
        m_selection->setTop(m_button->top() + m_button->height());
        m_selection->setHeight(height() - (m_button->top() + m_button->height() + m_offset));
        onValueChanged(this);
}

void VSlider::onHeightChange() {
        onSliderButtonMove();
}

} //namespace imEngine
