#include "BaseApplication.h"
#include "../Utils/Debug.h"

namespace imCore {

int BaseApplication::exec() {
        if (!init()) {
                IM_ERROR("Cannot init application");
                return -1;
        }
        loop();
        quit();
        return 0;
}

bool BaseApplication::init() {
        return false;
}

void BaseApplication::loop() {

}

void BaseApplication::quit() {

}

} //namespace imCore
