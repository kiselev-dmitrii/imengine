#include <imCore/System/BaseApplication.h>
#include <imCore/Utils/Debug.h>

using namespace imCore;

class TestApplication : public imCore::BaseApplication {
protected:
        void initialize() {
                IM_TRACE("Application initialized");
        }

        void update() {
                //IM_TRACE("Objects updated");

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

};

int main() {
        TestApplication app;
        return app.exec();
}
