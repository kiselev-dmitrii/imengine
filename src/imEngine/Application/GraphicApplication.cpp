#include "GraphicApplication.h"

namespace imEngine {

GuiManager* GraphicApplication::gui() const {
        return m_gui;
}


void GraphicApplication::initialize() {
        glClearColor(1,1,1,1);
        m_gui = new GuiManager("resources/gui/elementary/", this);
        m_fps = new FpsViewer(m_gui->root());
}

void GraphicApplication::update() {
        m_gui->processUpdate();
}

void GraphicApplication::render() {
        Renderer::clearBuffers();
        m_gui->processRender();
}

void GraphicApplication::destroy() {
        delete m_gui;
}

void GraphicApplication::mouseMoveEvent(int oldX, int oldY, int newX, int newY) {
        m_gui->processMouseMove(oldX, oldY, newX, newY);
}

void GraphicApplication::mousePressEvent(int x, int y, char button) {
        m_gui->processMousePress(x, y, button);
}

void GraphicApplication::mouseReleaseEvent(int x, int y, char button) {
        m_gui->processMouseRelease(x, y, button);
}

void GraphicApplication::keyPressEvent(int key) {
        m_gui->processKeyPress(key);

        switch (key) {
                case SDLK_F11:
                        mainWindow()->setFullscreen(!mainWindow()->isFullscreen());
                        break;
        }
}

void GraphicApplication::keyReleaseEvent(int key) {
        m_gui->processKeyRelease(key);
}

void GraphicApplication::windowResizeEvent(int x, int y) {
        glViewport(0, 0, x, y);
}

} //namespace imEngine
