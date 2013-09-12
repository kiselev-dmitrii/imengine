#include "BaseApplication.h"
#include "../Debug/Debug.h"
#include <GLFW/glfw3.h>

namespace imCore {

int BaseApplication::exec() {
        if (!initBaseApplication()) {
                IM_ERROR("Cannot init application");
                return -1;
        }
        runMainLoop();
        terminateApplication();
        return 0;
}

bool BaseApplication::initBaseApplication() {
        return false;
}

void BaseApplication::runMainLoop() {

}

void BaseApplication::terminateApplication() {

}

} //namespace imCore
