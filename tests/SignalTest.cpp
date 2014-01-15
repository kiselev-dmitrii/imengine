#include <imEngine/Graphics/GUI/Signal.h>
#include <imEngine/Utils/Debug.h>

using namespace imEngine;

class WgtSlider {
public:
        WgtSlider(int value) : m_value(value), m_size(10, 20)                       { }

        void setSize(Vec2 size) {
                m_size = size;
                onSizeChanged(size);
        }

        void setValue(int value) {
                m_value = value;
                onValueChanged(value);
        }

public:
        Signal<Vec2>    onSizeChanged;
        Signal<int>     onValueChanged;

private:
        int             m_value;
        Vec2            m_size;
};

class WgtLabel {
public:
        WgtLabel(const String& text) : m_text(text)                              { }

        void    setText(const String& text) {
                IM_TRACE(text);
                m_text = text;
        }
private:
        String  m_text;
};

void sizeChanged(const Vec2& size) {
        IM_TRACE(size);
}

int main() {
        WgtSlider slider(0);
        WgtLabel  label("");

        slider.onValueChanged += [&] (int value) { label.setText(std::to_string(value)); };
        slider.onSizeChanged += sizeChanged;

        slider.setValue(10);
        slider.setSize(Vec2(100, 300));

        return 0;
}
