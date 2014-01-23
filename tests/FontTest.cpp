#include <imEngine/Application/BaseApplication.h>
#include <imEngine/FileContainers/Font.h>
#include <imEngine/Utils/Debug.h>

using namespace imEngine;

class Application : public BaseApplication {
protected:
        void    initialize();
        void    update();
        void    render();
        void    destroy();

        void    keyPressEvent(int key);

private:
        FontPtr         m_font;

};

void Application::initialize() {
        glClearColor(0,0,0,1);

        m_font = Font::defaultFont();
        m_font = FontPtr(new Font("resources/font/Lobster.ttf", 48));

        // Проверка значения maxHeight
        int maxHeight = 0;
        for (uint ch = 32; ch <= 127; ++ch) {
                Glyph g = m_font->glyph(ch);
                maxHeight = std::max(maxHeight, g.size.y);
        }
        IM_VAR(maxHeight);
        IM_VAR(m_font->maxHeight());

        // Проверка значения descenderHeight
        int descender = 0;
        for (uint ch = 32; ch <= 127; ++ch) {
                Glyph g = m_font->glyph(ch);
                int curOffset = g.size.y - g.bearing.y;
                descender = std::max(descender, curOffset);
        }
        IM_VAR(descender);
        IM_VAR(m_font->descenderHeight());

        IM_VAR(m_font->texture()->width());
        IM_VAR(m_font->texture()->height());

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Application::update() {
}

void Application::render() {
}

void Application::keyPressEvent(int key) {
        if (!m_font->isVisibleGlyph(key)) return;

        Glyph g = m_font->glyph(key);
        IM_VAR(g.size);
        IM_VAR(g.advance);
        IM_VAR(g.bearing);
        IM_VAR(g.texCoords);
}

void Application::destroy() {
}

int main() {
        Application app;
        app.exec();
}
