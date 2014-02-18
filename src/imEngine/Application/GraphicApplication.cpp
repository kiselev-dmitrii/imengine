#include "GraphicApplication.h"
#include <imEngine/Graphics/Scene/SceneDeferred.h>

namespace imEngine {

GuiManager* GraphicApplication::gui() const {
        return m_gui;
}

Scene* GraphicApplication::scene() const {
        return m_scene;
}

void GraphicApplication::initialize() {
        glClearColor(1,1,1,1);
        Renderer::setViewportSize(window()->size());

        m_scene = new SceneDeferred(this);
        m_gui = new GuiManager("resources/gui/elementary/", this);
        m_fps = new FpsViewer(m_gui->root());
}

void GraphicApplication::update() {
        static float prevTime = currentTime();
        float curTime = currentTime();
        float delta = curTime - prevTime;
        prevTime = curTime;

        m_scene->update(delta);
        m_gui->processUpdate();
}

void GraphicApplication::render() {
        Renderer::clearBuffers();
        m_scene->render();
        m_gui->processRender();
}

void GraphicApplication::destroy() {
        delete m_scene;
        delete m_gui;
}

void GraphicApplication::mouseMoveEvent(int oldX, int oldY, int newX, int newY) {
        m_gui->processMouseMove(oldX, oldY, newX, newY);
}

void GraphicApplication::mousePressEvent(int x, int y, char button) {
        m_gui->processMousePress(x, y, button);
        m_scene->mousePressEvent(x, y, button);
}

void GraphicApplication::mouseReleaseEvent(int x, int y, char button) {
        m_gui->processMouseRelease(x, y, button);
}

void GraphicApplication::keyPressEvent(int key) {
        m_gui->processKeyPress(key);

        switch (key) {
                case SDLK_F11:
                        window()->setFullscreen(!window()->isFullscreen());
                        break;
        }
}

void GraphicApplication::keyReleaseEvent(int key) {
        m_gui->processKeyRelease(key);
}

void GraphicApplication::windowResizeEvent(int x, int y) {
        Renderer::setViewportSize(IVec2(x, y));
        m_scene->windowResizeEvent(x, y);
}

} //namespace imEngine
