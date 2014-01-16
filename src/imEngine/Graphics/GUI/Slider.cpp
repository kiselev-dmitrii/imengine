#include "Slider.h"
#include "Button.h"
#include <imEngine/Utils/Debug.h>

namespace imEngine {

/** @brief Обобщенная кнопка для слайдера
 */
class SliderButton : public Button {
public:
        SliderButton(const String& active, const String& hover, float bound, WidgetAbstract* parent) :
                Button(active, hover, hover, active, active, parent),
                m_mousePosPrev(0,0),
                m_bound(bound),
                m_percent(0)
        { }

        bool onMousePress(int x, int y, char button) {
                if (button != MouseButton::LEFT) return false;
                Button::onMousePress(x,y, button);
                m_mousePosPrev = Vec2(x,y) - absolutePosition();
                return true;
        }

        float percent() const {
                return m_percent;
        }

        virtual void setOffset(float offset) = 0;

protected:
        Vec2            m_mousePosPrev;
        float           m_bound;
        float           m_percent;
};


/** @brief Кнопка для горизонтального слайдера
 */
class HSliderButton : public SliderButton {
public:
        HSliderButton(const String& active, const String& hover, float bound, HSlider* parent) : SliderButton(active, hover, bound, parent) { }

        void onGlobalMouseMove(int x, int y) {
                if (!isPressed()) return;

                Vec2  pos = position();
                float start = m_bound;
                float end = static_cast<WidgetAbstract*>(parent())->width() - m_bound - width();

                if (pos.x >= start && pos.x <= end) {
                        // Устанавливаем новую позицию
                        Vec2 mousePosNew = Vec2(x,y) - absolutePosition();
                        pos.x += mousePosNew.x - m_mousePosPrev.x;
                        pos.x = glm::clamp(pos.x, start, end);
                        setPosition(pos);

                        // Вычисляем значение
                        m_percent = (pos.x-start)/(end-start);
                        static_cast<HSlider*>(parent())->updateSelection();
                }
        }

        void setOffset(float offset) {
                float start = m_bound;
                float end = static_cast<WidgetAbstract*>(parent())->width() - m_bound - width();
                if (offset >= start && offset <= end) setLeft(offset);
        }
};


/** @brief Кнопка для вертикального слайдера
 */
class VSliderButton : public SliderButton {
public:
        VSliderButton(const String& active, const String& hover, float bound, VSlider* parent) : SliderButton(active, hover, bound, parent) { }

        void onGlobalMouseMove(int x, int y) {
                if (!isPressed()) return;

                Vec2  pos = position();
                float start = m_bound;
                float end = static_cast<WidgetAbstract*>(parent())->height() - m_bound - height();

                if (pos.y >= start && pos.y <= end) {
                        // Устанавливаем новую позицию
                        Vec2 mousePosNew = Vec2(x,y) - absolutePosition();
                        pos.y += mousePosNew.y - m_mousePosPrev.y;
                        pos.y = glm::clamp(pos.y, start, end);
                        setPosition(pos);

                        // Вычисляем значение
                        m_percent = (pos.y-start)/(end-start);
                        static_cast<VSlider*>(parent())->updateSelection();
                }
        }

        void setOffset(float offset) {
                float start = m_bound;
                float end = static_cast<WidgetAbstract*>(parent())->height() - m_bound - height();
                if (offset >= start && offset <= end) setTop(offset);
        }
};


//################################ HSlider ###################################//


HSlider::HSlider(const String &sliderBackground, const String &sliderSelection, const String &btnActive, const String &btnHover, WidgetAbstract *parent) :
        HStretchableTexturedWidget(sliderBackground, parent)
{
        // Создаем область с выделением, и устанавливаем ее сразу после угла слайдера
        m_selection = new HStretchableTexturedWidget(sliderSelection, this);
        m_selection->setLeft(minimalWidth()/2);

        // Создаем кнопку после выделения, чтобы она рендерилась позже
        m_button = new HSliderButton(btnActive, btnHover, minimalWidth()/2, this);
        m_button->setLeft(minimalWidth()/2);
        m_button->setTop(height()/2 - m_button->height()/2);
}

void HSlider::updateSelection() {
        m_selection->setWidth(m_button->position().x-5);
}

bool HSlider::onMousePress(int x, int y, char button) {
        Vec2 m = Vec2(x,y) - absolutePosition();
        m_button->setOffset(m.x);
        updateSelection();
        return true;
}


//################################ VSlider ###################################//


} //namespace imEngine
