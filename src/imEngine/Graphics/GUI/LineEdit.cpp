#include "LineEdit.h"
#include <imEngine/Utils/Debug.h>
#include <imEngine/Application/GraphicApplication.h>
#include <imEngine/Graphics/PrimitiveRenderer.h>

namespace imEngine {

LineEdit::LineEdit(const String &active, const String &disabled, const String &focused, WidgetAbstract *parent) :
        HStretchableTexturedWidget(active, parent),
        m_text(nullptr),
        m_activeImage(active),
        m_disabledImage(disabled),
        m_focusedImage(focused),
        m_isCursorVisible(false),
        m_isCursorBlinking(false),
        m_prevBlinkTime(manager()->application()->currentTime()),
        m_isCursorEnabled(true)
{
        m_text = new Text("", this);
}

void LineEdit::setCursorEnabled(bool isEnabled) {
        m_isCursorEnabled = isEnabled;
}

bool LineEdit::isCursorEnabled() const {
        return m_isCursorEnabled;
}

void LineEdit::setText(const String &text) {
        if (m_text->font()->calculateWidthOfText(text) > contentWidth()) return;
        m_text->setText(text);
}

String LineEdit::text() const {
        return m_text->text();
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
        setCursorBlinking(true);
}

void LineEdit::onWidgetClearFocus() {
        setCurrentImage(m_activeImage);
        setCursorBlinking(false);
}

void LineEdit::onKeyPress(int key) {
        processCharKeys(key);
        processControlKeys(key);
        setCursorVisible(true);
}

void LineEdit::onUpdate() {
        if (m_isCursorBlinking)  {
                if (manager()->application()->currentTime() - m_prevBlinkTime > 1.0) blinkByCursor();
        }
}

void LineEdit::onRender() {
        HStretchableTexturedWidget::onRender();

        if (m_isCursorVisible && m_isCursorEnabled) {
                Vec2 pos = m_text->absolutePosition() + Vec2(m_text->width(), 0);
                Vec2 sz = Vec2(2, m_text->height());
                PrimitiveRenderer::instance().drawRectangleInScreenSpace(pos, sz, m_text->color(), manager()->window());
        }
}

void LineEdit::processCharKeys(char ch) {
        if (manager()->window()->keyboard()->modifiers() & KeyboardModifiers::SHIFT) ch = std::toupper(ch);
        if (m_text->font()->isVisibleGlyph(ch)) {
                setText(m_text->text() + ch);
                return;
        }
}

void LineEdit::processControlKeys(int key) {
        String text = m_text->text();

        switch (key) {
                case SDLK_BACKSPACE:
                        if (!text.empty()) {
                                text.erase(text.end()-1);
                                m_text->setText(text);
                        }
                        break;
                case SDLK_RETURN:
                case SDLK_ESCAPE:
                        clearFocus();
                        break;
        }
}

void LineEdit::blinkByCursor() {
        setCursorVisible(!m_isCursorVisible);
}

void LineEdit::setCursorVisible(bool isVisible) {
        m_isCursorVisible = isVisible;
        m_prevBlinkTime = manager()->application()->currentTime();
}

void LineEdit::setCursorBlinking(bool isBlinking) {
        if (isBlinking) setCursorVisible(true);
        else setCursorVisible(false);
        m_isCursorBlinking = isBlinking;
}

} //namesapce imEngine
