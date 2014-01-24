#include "LineEdit.h"
#include <imEngine/Utils/Debug.h>

namespace imEngine {

LineEdit::LineEdit(const String &active, const String &disabled, const String &focused, WidgetAbstract *parent) :
        HStretchableTexturedWidget(active, parent),
        m_text(nullptr),
        m_activeImage(active),
        m_disabledImage(disabled),
        m_focusedImage(focused)
{
        m_text = new Text("", this);
        m_text->alignVertical(WidgetVAlignment::CENTER);
}

bool LineEdit::onMousePress(int x, int y, char button) {
        setFocus();
        return true;
}

void LineEdit::onGlobalMousePress(int x, int y, char button) {
        if (this->isInsideWidget(x,y)) return;
        clearFocus();
}

void LineEdit::onWidgetSetFocus() {
        setCurrentImage(m_focusedImage);
}

void LineEdit::onWidgetClearFocus() {
        setCurrentImage(m_activeImage);
}

void LineEdit::onKeyPress(int key) {
        String currentText = m_text->text();

        if (m_text->font()->isVisibleGlyph(key)) {
                m_text->setText(currentText + char(key));
                return;
        }

       if (key == SDLK_BACKSPACE) {
               currentText.erase(currentText.end()-1);
               m_text->setText(currentText);
       }
}

} //namesapce imEngine
