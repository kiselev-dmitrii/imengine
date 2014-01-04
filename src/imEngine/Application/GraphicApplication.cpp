#include "GraphicApplication.h"

namespace imEngine {

void GraphicApplication::initialize() {
        initFPS();
}

void GraphicApplication::initFPS() {
        m_fps = 0;
        m_oldTime = currentTime();
        m_textFps = new Text("FPS: 0", FontPtr(new Font("resources/font/FreeSans.ttf", 23)), mainWindow());
        m_textFps->setPosition(Vec2(30,30));
        m_textFps->setColor(Vec3(1,1,0));
}

void GraphicApplication::update() {
        updateFPS();
}

void GraphicApplication::updateFPS() {
        ++m_fps;
        if ((currentTime() - m_oldTime) >= 1.0) {
                m_textFps->setText("FPS: " + std::to_string(m_fps));
                m_fps = 0;
                m_oldTime = currentTime();
        }
}

void GraphicApplication::render() {
        Renderer::clearBuffers();
        m_textFps->render();
}

void GraphicApplication::destroy() {
        delete m_textFps;
}

void GraphicApplication::keyPressEvent(int key) {
        switch (key) {
                case SDLK_F11:
                        mainWindow()->setFullscreen(!mainWindow()->isFullscreen());
                        break;
        }

}

uint GraphicApplication::fps() const {
        return m_fps;
}

} //namespace imEngine
