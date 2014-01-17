#include "SliderButton.h"
#include <imEngine/Utils/Debug.h>

namespace imEngine {

SliderButton::SliderButton(const String &active, const String &hover, const Vec2 &start, const Vec2 &end, WidgetAbstract *parent) :
        Button(active, hover, hover, active, active, parent),
        m_prevMousePos(0,0),
        m_start(start),
        m_end(end),
        m_percent(0)
{
        setPercent(0);
}

bool SliderButton::onMousePress(int x, int y, char button) {
        if (button != MouseButton::LEFT) return false;
        Button::onMousePress(x, y, button);
        m_prevMousePos = absoluteToLocal(Vec2(x,y));
        return true;
}

void SliderButton::onGlobalMouseMove(int x, int y) {
        if (!isPressed()) return;

        Vec2 pos = position();
        if ((pos.x >= m_start.x && pos.y >= m_start.y) && (pos.x <= m_end.x && pos.y <= m_end.y)) {
                // приращение мыши
                Vec2 deltaMouse = absoluteToLocal(Vec2(x,y)) - m_prevMousePos;
                // направление слайдера и приращения мыши
                Vec2 sliderDir = glm::normalize(m_end - m_start);
                Vec2 mouseDeltaDir = glm::normalize(deltaMouse);
                // Проекция приращения мыши на ось слайдера
                float deltaLen = deltaMouse.length()*glm::dot(sliderDir, mouseDeltaDir);
                // Приращение позиции кнопки
                Vec2 delta = deltaLen * sliderDir;
                // Новая позиция
                pos += delta;
                pos = glm::clamp(pos, m_start, m_end);
                setPosition(pos);

                // Вычисление позиции кнопки в процентах
                m_percent = (pos-m_start).length() / (m_end-m_start).length();
        }
}

void SliderButton::setBounds(const Vec2 &start, const Vec2 &end) {
        m_start = start;
        m_end = end;
        setPosition(m_start + (m_end-m_start)*m_percent);
}

Vec2 SliderButton::startBound() const {
        return m_start;
}

Vec2 SliderButton::endBound() const {
        return m_end;
}

float SliderButton::percent() const {
        return m_percent;
}

void SliderButton::setPercent(float percent) {
        m_percent = glm::clamp(percent, 0.0f, 1.0f);
        setPosition(m_start + (m_end-m_start)*m_percent);
}

} //namespace imEngine
