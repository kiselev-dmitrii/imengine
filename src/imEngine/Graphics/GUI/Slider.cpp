#include "Slider.h"

namespace imEngine {

SliderButton::SliderButton(const String &active, const String &hover, WidgetAbstract *parent) :
        NonStretchableTexturedWidget(active, parent),
        m_activeImage(active),
        m_hoverImage(hover),
        m_wasPressed(false),
        m_mouseOffset(0,0)
{}

bool SliderButton::onMouseEnter(int x, int y) {
        setCurrentImage(m_hoverImage);
        return true;
}

bool SliderButton::onMouseLeave(int x, int y) {
        setCurrentImage(m_activeImage);
        return true;
}

bool SliderButton::onMousePress(int x, int y, char button) {
        if (button != MouseButton::LEFT) return false;

        m_wasPressed = true;
        m_mouseOffset = Vec2(x,y) - absolutePosition();
        setCurrentImage(m_hoverImage);
        return true;
}

bool SliderButton::onMouseRelease(int x, int y, char button) {
        if (button != MouseButton::LEFT) return false;

        m_wasPressed = false;
        setCurrentImage(m_activeImage);
        return true;
}

bool SliderButton::onMouseMove(int x, int y) {
        Vec2 pos = absolutePosition();
        if (m_wasPressed) setAbsolutePosition(Vec2(x - m_mouseOffset.x, pos.y));
        return true;
}

} //namespace imEngine
