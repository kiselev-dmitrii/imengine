#include "Debug.h"
#include "GLUtils.h"
#include <ctime>

namespace imCore {

String Debug::currentTimeStamp() {
        clock_t time = clock();
        clock_t hours = time / (CLOCKS_PER_SEC * 3600);
        clock_t minutes = time / (CLOCKS_PER_SEC * 60);
        clock_t seconds = time / CLOCKS_PER_SEC;
        clock_t milliseconds = time % CLOCKS_PER_SEC;
        seconds -= minutes * 60;
        minutes -= hours * 60;

        String result = "[" + std::to_string(hours) + ":" +
                              std::to_string(minutes) + ":" +
                              std::to_string(seconds) + ":"+
                        std::to_string(milliseconds) + "]";
        return result;
}

void Debug::checkGlErrors(const String &function, const String &file, uint line, const String &call) {
        GLenum error = glGetError();
        if (error == GL_NO_ERROR) return;
        std::clog << currentTimeStamp() <<
                     " GLError: Function: " << function << "(...) ,  File: " << file << ", Line: " << line <<
                     "Cause: " << "Call " << call << " return " << GLUtils::convertEnumToString(error) << std::endl;
}

} //namespace imCore
