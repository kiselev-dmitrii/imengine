#include "Label.h"
#include <imEngine/Utils/Debug.h>

namespace imEngine {

Label::Label(const String &text, Widget *parent) :
        Widget(parent), HStretchableAbstract(),
        m_alignment(LabelAlignment::LEFT),
        m_tmpString(text)
{ }

void Label::initialize(GuiManager *manager) {
        Widget::initialize(manager);
        m_text = TextPtr(new Text(m_tmpString, Font::defaultFont(), manager->window()));
        m_size = Vec2(m_text->font()->calcSizeOfText(m_text->text()));
        setMinimalWidth(0);
}

void Label::update() {  }

void Label::render() {
        Vec2 wgtPos = absolutePosition();
        Vec2 wgtSize = size();
        float txtWidth = m_text->size().x;

        // Вычисляем позицию текста
        Vec2 txtPos;
        switch (m_alignment) {
                case LabelAlignment::LEFT:
                        txtPos.x = wgtPos.x;
                        break;
                case LabelAlignment::CENTER:
                        txtPos.x = wgtPos.x + wgtSize.x/2 - txtWidth/2;
                        break;
                case LabelAlignment::RIGHT:
                        txtPos.x = wgtPos.x + wgtSize.x - txtWidth;
                        break;
        }
        txtPos.y = wgtPos.y + wgtSize.y/2;
        m_text->setPosition(txtPos);

        m_text->render();

        renderChildren();
}

} //namespace imEngine
