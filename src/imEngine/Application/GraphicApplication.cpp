#include "GraphicApplication.h"

namespace imEngine {

uint GraphicApplication::fps() const {
        return m_fps;
}

GuiManager* GraphicApplication::gui() const {
        return m_gui;
}


void GraphicApplication::initialize() {
        glClearColor(1,1,1,1);
        m_gui = new GuiManager("resources/gui/elementary/", mainWindow());
        initFPS();
}

void GraphicApplication::update() {
        updateFPS();
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

void GraphicApplication::initFPS() {
        m_fps = 0;
        m_oldTime = currentTime();
        m_textFps = new Text("FPS: 0", gui()->root());
}

void GraphicApplication::updateFPS() {
        ++m_fps;
        if ((currentTime() - m_oldTime) >= 1.0) {
                m_textFps->setText("FPS: " + std::to_string(m_fps));
                m_fps = 0;
                m_oldTime = currentTime();
        }
}


} //namespace imEngine
