#include "LineEdit.h"
#include <imEngine/Utils/Debug.h>
#include <imEngine/Application/GraphicApplication.h>
#include <imEngine/Graphics/PrimitiveRenderer.h>

namespace imEngine {

//######################### LineEditCursor ###################################//

LineEditCursor::LineEditCursor(Text* text, GraphicApplication* application) :
        m_isVisible(false),
        m_isBlinking(false),
        m_prevBlinkTime(application->currentTime()),
        m_text(text),
        m_application(application)
{ }

void LineEditCursor::setBlinking(bool isBlinking) {
        if (isBlinking) setVisible(true);
        else setVisible(false);
        m_isBlinking = isBlinking;
}

void LineEditCursor::setVisible(bool isVisible) {
        m_isVisible = isVisible;
        m_prevBlinkTime = m_application->currentTime();
}

void LineEditCursor::blink() {
        setVisible(!m_isVisible);
}

void LineEditCursor::update() {
        if (m_isBlinking) {
                if (m_application->currentTime() - m_prevBlinkTime > 1.0) blink();
        }
}

void LineEditCursor::render() {
        if (m_isVisible) {
                Vec2 pos(m_text->absolutePosition() + Vec2(m_text->width(), 0));
                Vec2 size(2, m_text->height());
                PrimitiveRenderer::instance().drawRectangleInScreenSpace(pos, size, m_text->color(), m_application->mainWindow());
        }
}

//############################ LineEdit ######################################//

LineEdit::LineEdit(const String &active, const String &disabled, const String &focused, WidgetAbstract *parent) :
        HStretchableTexturedWidget(active, parent),
        m_text(new Text("", this)),
        m_activeImage(active),
        m_disabledImage(disabled),
        m_focusedImage(focused),
        m_cursor(m_text, manager()->application()),
        m_isCursorVisible(true)
{ }

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
        m_cursor.setBlinking(true);
}

void LineEdit::onWidgetClearFocus() {
        setCurrentImage(m_activeImage);
        m_cursor.setBlinking(false);
}

void LineEdit::onKeyPress(int key) {
        processCharKeys(key);
        processControlKeys(key);

        m_cursor.setVisible(true);
}

void LineEdit::onUpdate() {
        m_cursor.update();
}

void LineEdit::onRender() {
        HStretchableTexturedWidget::onRender();
        if (m_isCursorVisible) m_cursor.render();
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

} //namesapce imEngine
