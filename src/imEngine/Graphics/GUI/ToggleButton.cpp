#include "ToggleButton.h"

namespace imEngine {

ToggleButton::ToggleButton(const String &active, const String &checked, const String &disabledActive,
                           const String &disabledChecked, WidgetAbstract *parent) :
        BothStretchableTexturedWidget(active, parent),
        m_activeImage(active),
        m_checkedImage(checked),
        m_disabledActiveImage(disabledActive),
        m_disabledCheckedImage(disabledChecked),
        m_isPressed(false),
        m_isChecked(false)
{ }

void ToggleButton::setChecked(bool checked) {
        m_isChecked = checked;
}

bool ToggleButton::isChecked() const {
        return m_isChecked;
}

bool ToggleButton::onMousePress(int x, int y, char button) {
        if (button != MouseButton::LEFT) return false;

        m_isPressed = true;
        return true;
}

bool ToggleButton::onMouseRelease(int x, int y, char button) {
        if (button != MouseButton::LEFT) return false;

        if (m_isPressed) {
                m_isChecked = !m_isChecked;
                if (m_isChecked) setCurrentImage(m_checkedImage);
                else setCurrentImage(m_activeImage);

                m_isPressed = false;
        }
        return true;
}

void ToggleButton::onGlobalMouseRelease(int x, int y, char button) {
        if (button != MouseButton::LEFT) return;

        m_isPressed = false;
}

} //namespace imEngine