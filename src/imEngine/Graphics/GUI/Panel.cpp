#include "Panel.h"
#include <imEngine/Utils/Debug.h>

namespace imEngine {

Panel::Panel(const String& image, WidgetAbstract* parent) :
        BothStretchableTexturedWidget(image, parent),
        m_wasMousePressed(false),
        m_mouseOffset(0,0)
{ }

bool Panel::onMousePress(int x, int y, char button) {
        if (button != MouseButton::RIGHT) return false;

        m_wasMousePressed = true;
        m_mouseOffset = Vec2(x,y) - absolutePosition();

        return true;
}

bool Panel::onMouseRelease(int x, int y, char button) {
        if (button != MouseButton::RIGHT) return false;

        m_wasMousePressed = false;

        return true;
}

bool Panel::onMouseMove(int x, int y) {
        if (m_wasMousePressed) setAbsolutePosition(Vec2(x,y) - m_mouseOffset);
        return true;
}


} //namesapce imEngine
