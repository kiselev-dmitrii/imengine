#include "Slider.h"
#include "Button.h"
#include <imEngine/Utils/Debug.h>

namespace imEngine {

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

bool HSlider::onMousePress(int x, int y, char button) {
        m_button->setValidPosition(absoluteToLocal(Vec2(x,y)));
        m_button->onMousePress(x, y, MouseButton::LEFT);
        return true;
}

void HSlider::onSliderButtonMove() {
        m_selection->setWidth(m_button->left() - m_button->width()/2);
}

//################################ VSlider ###################################//


} //namespace imEngine
