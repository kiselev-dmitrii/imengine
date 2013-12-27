#include "GraphicApplication.h"

namespace imEngine {

void GraphicApplication::initialize() {

}

void GraphicApplication::update() {

}

void GraphicApplication::render() {

}

void GraphicApplication::destroy() {

}

void GraphicApplication::keyPressEvent(int key) {
        switch (key) {
                case SDLK_F11:
                        mainWindow()->setFullscreen(!mainWindow()->isFullscreen());
                        break;
        }

}

ResourceManager* GraphicApplication::resources() {
        return &m_resourceManager;
}

} //namespace imEngine
