#include <imEngine/Application/BaseApplication.h>
#include <imEngine/Utils/Debug.h>

using namespace imEngine;

class TestApplication : public imEngine::BaseApplication {
protected:
        void initialize() {
                IM_TRACE("Application initialized");
        }

        void update() {
                //IM_TRACE("Objects updated");
                testKeyboard();
                testMouse();
        }

        void render() {
                //IM_TRACE("Objects rendered");
        }

        void destroy() {
                IM_TRACE("Application destroyed");
        }

        void keyPressEvent(int key) {
                IM_TRACE("");
                IM_VAR(key);
        }

        void keyReleaseEvent(int key) {
                IM_TRACE("");
                IM_VAR(key);
        }

        void mousePressEvent(int x, int y, char button) {
                IM_TRACE("");
                IM_VAR(x);
                IM_VAR(y);
                IM_VAR(button);
        }

        void mouseReleaseEvent(int x, int y, char button) {
                IM_TRACE("");
                IM_VAR(x);
                IM_VAR(y);
                IM_VAR(button);
        }

        void mouseWheelEvent(int deltaVertical, int deltaHorizontal) {
                IM_TRACE("");
                IM_VAR(deltaVertical);
                IM_VAR(deltaHorizontal);
        }

        void windowResizeEvent(int x, int y) {
                IM_TRACE("");
                IM_VAR(x);
                IM_VAR(y);
        }

        void windowMinimizeEvent() {
                IM_TRACE("");
        }

        void testKeyboard() {
                Keyboard* keyboard = mainWindow()->keyboard();
                if (keyboard->isKeyPressed(SDLK_UP)) mainWindow()->setPosition(mainWindow()->position() + IVec2(0, -1));
                if (keyboard->isKeyPressed(SDLK_DOWN)) mainWindow()->setPosition(mainWindow()->position() + IVec2(0, 1));
                if (keyboard->isKeyPressed(SDLK_LEFT)) mainWindow()->setPosition(mainWindow()->position() + IVec2(-1, 0));
                if (keyboard->isKeyPressed(SDLK_RIGHT)) mainWindow()->setPosition(mainWindow()->position() + IVec2(1, 0));

                if (keyboard->modifiers() & KeyboardModifiers::ALT) IM_PRINT("MODIFIER: ALT PRESSED");
        }

        void testMouse() {
                Keyboard* keyboard = mainWindow()->keyboard();
                Mouse* mouse = mainWindow()->mouse();
                if (keyboard->isKeyPressed(SDLK_c)) mouse->setPosition(mainWindow()->center());
                if (keyboard->isKeyPressed(SDLK_h)) mouse->setVisible(false);
                if (keyboard->isKeyPressed(SDLK_s)) mouse->setVisible(true);
                if (mouse->isButtonPressed(MouseButton::MIDDLE)) {
                        IM_VAR(mouse->isVisible());
                        IM_VAR(mouse->position());
                }
        }

};

int main() {
        TestApplication app;
        return app.exec();
}
