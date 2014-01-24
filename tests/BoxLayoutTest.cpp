#include <imEngine/Application/GraphicApplication.h>
#include <imEngine/Graphics/GUI/GuiManager.h>
#include <imEngine/Graphics/GUI/Button.h>
#include <imEngine/Graphics/GUI/ToggleButton.h>
#include <imEngine/Graphics/GUI/Panel.h>
#include <imEngine/Graphics/GUI/BoxLayout.h>
#include <imEngine/Graphics/GUI/Slider.h>
#include <imEngine/Graphics/GUI/Picture.h>
#include <imEngine/Graphics/GUI/TextButton.h>
#include <imEngine/Graphics/GUI/LineEdit.h>
#include <imEngine/Utils/Debug.h>

using namespace imEngine;

class Application : public GraphicApplication {
protected:
        void    initialize();
        void    keyPressEvent(int key);

private:
        Panel*          m_pnl;
        TextButton*     m_btnPlus;
        TextButton*     m_btnMinus;
        HSlider*        m_slider;
        HBoxLayout*     m_lt;

        TextButton*     m_btn1;
        TextButton*     m_btn2;
        TextButton*     m_btn3;
        ToggleButton*   m_toggle;

        VBoxLayout*     m_vlt;

        LineEdit*       m_edt1;
        LineEdit*       m_edt2;
        VBoxLayout*     m_edits;

        Picture*        m_pic;
};

void Application::initialize() {
        GraphicApplication::initialize();

        // Создаем панельку
        m_pnl = new Panel("regular_panel.png", gui()->root());
        m_pnl->setSize(Vec2(700, 500));

        // Создаем HBoxLayout
        m_lt = new HBoxLayout(m_pnl);
        m_lt->setPosition(Vec2(100, 200));

        // Создание кнопок и слайдера
        m_btnMinus = new TextButton("Decrease", "regular_btn_active.png", "regular_btn_hover.png", "regular_btn_pressed.png", "regular_btn_disabled.png", "regular_btn_focused.png", m_lt);
        m_lt->addWidget(m_btnMinus, WidgetVAlignment::CENTER);

        m_slider = new HSlider("slider_background.png", "slider_selection.png", "slider_btn_active.png", "slider_btn_hover.png", m_lt);
        m_slider->setWidth(300);
        m_lt->addWidget(m_slider, WidgetVAlignment::CENTER);

        m_btnPlus = new TextButton("Increase", "regular_btn_active.png", "regular_btn_hover.png", "regular_btn_pressed.png", "regular_btn_disabled.png", "regular_btn_focused.png", m_lt);
        m_lt->addWidget(m_btnPlus, WidgetVAlignment::CENTER);


        // Создаем VBoxLayout и цепляем к нему 3 кнопки
        m_vlt = new VBoxLayout(m_lt);
        m_vlt->setPosition(Vec2(30,30));

        m_btn1 = new TextButton("First", "regular_btn_active.png", "regular_btn_hover.png", "regular_btn_pressed.png", "regular_btn_disabled.png", "regular_btn_focused.png", m_vlt);
        m_vlt->addWidget(m_btn1);

        m_btn2 = new TextButton("Second", "regular_btn_active.png", "regular_btn_hover.png", "regular_btn_pressed.png", "regular_btn_disabled.png", "regular_btn_focused.png", m_vlt);
        m_vlt->addWidget(m_btn2);

        m_btn3 = new TextButton("Third", "regular_btn_active.png", "regular_btn_hover.png", "regular_btn_pressed.png", "regular_btn_disabled.png", "regular_btn_focused.png", m_vlt);
        m_vlt->addWidget(m_btn3);

        m_toggle = new ToggleButton("big_checkbox_active.png", "big_checkbox_checked.png", "big_checkbox_active.png", "big_checkbox_checked.png", m_vlt);
        m_vlt->addWidget(m_toggle, WidgetHAlignment::CENTER);

        m_lt->insertWidget(m_vlt, 0);

        m_edits = new VBoxLayout(m_pnl);
        m_edits->setPosition(Vec2(40,40));

        m_edt1 = new LineEdit("regular_lineedit_active.png", "regular_lineedit_disabled.png", "regular_lineedit_focused.png", m_edits);
        m_edt1->setPadding(9);
        m_edt1->setWidth(300);
        m_edits->addWidget(m_edt1);

        m_edt2 = new LineEdit("regular_lineedit_active.png", "regular_lineedit_disabled.png", "regular_lineedit_focused.png", m_edits);
        m_edt2->setPadding(9);
        m_edt2->setWidth(300);
        m_edits->addWidget(m_edt2);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Application::keyPressEvent(int key) {
        GraphicApplication::keyPressEvent(key);

        if (key == '-') m_slider->setWidth(m_slider->width()-1);
        if (key == '=') m_slider->setWidth(m_slider->width()+1);
        if (key == '1') m_slider->setVisible(!m_slider->isVisible());
}

int main() {
        Application app;
        return app.exec();
}
