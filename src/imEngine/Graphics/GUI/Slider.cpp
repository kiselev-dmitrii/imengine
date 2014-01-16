#include "Slider.h"
#include <imEngine/Utils/Debug.h>

namespace imEngine {

SliderButton::SliderButton(const String &active, const String &hover, float bound, WidgetAbstract *parent) :
        Button(active, hover, hover, active, active, parent),
        m_mousePosPrev(0,0),
        m_bound(bound),
        m_percent(0)
{
        setPosition(Vec2(bound,0));
}


bool SliderButton::onMousePress(int x, int y, char button) {
        if (button != MouseButton::LEFT) return false;

        Button::onMousePress(x,y, button);
        m_mousePosPrev = Vec2(x,y) - absolutePosition();                //в локальных координатах
        return true;
}

void SliderButton::onGlobalMouseMove(int x, int y) {
        if (!isPressed()) return;

        Vec2 pos = position();
        float width = size().x;
        float parentWidth = ((WidgetAbstract*)parent())->size().x;
        float start = m_bound;
        float end = parentWidth - m_bound - width;

        if (pos.x >= start && pos.x <= end) {
                // Устанавливаем новую позицию
                Vec2 mousePosNew = Vec2(x,y) - absolutePosition();
                pos.x += mousePosNew.x - m_mousePosPrev.x;
                pos.x = glm::clamp(pos.x, start, end);
                setPosition(pos);

                // Вычисляем значение
                m_percent = (pos.x-start)/(end-start);
                onValueChanged(m_percent);
        }
}

void SliderButton::setOffset(float x) {
        float width = size().x;
        float parentWidth = ((WidgetAbstract*)parent())->size().x;
        float start = m_bound;
        float end = parentWidth - m_bound - width;

        if (x >= start && x <= end) {
                setLeft(x);
        }
}


//################################ HSlider ###################################//


HSlider::HSlider(const String &sliderBackground, const String &sliderSelection,
                 const String &btnActive, const String &btnHover, WidgetAbstract *parent) :
        HStretchableTexturedWidget(sliderBackground, parent)
{
        m_selection = new HStretchableTexturedWidget(sliderSelection, this);
        m_selection->setPosition(Vec2(minimalWidth()/2, 0));
        m_button = new SliderButton(btnActive, btnHover, minimalWidth()/2, this);
        m_button->setPosition(Vec2(minimalWidth()/2, -3));
        m_button->onValueChanged += [&] (float) { resizeSelection(); };
        setWidth(50);
}

void HSlider::resizeSelection() {
        IM_TRACE("");
        m_selection->setWidth(m_button->position().x-5);
}

bool HSlider::onMousePress(int x, int y, char button) {
        Vec2 m = Vec2(x,y) - absolutePosition();
        m_button->setOffset(m.x);
        m_button->onValueChanged(m_button->percent());
        return true;
}

} //namespace imEngine
