#include <imCore/Debug/Logger.h>

using namespace imCore;

int main() {
        Logger* logger = Logger::instance();
        logger->addMessage("MASSIVE OPERATIONS:");
        logger->addTimestampMessage("Start of long operation");
        for (long long i=0; i < 34*100500; ++i) {
                int* arr = new int [100500];
                delete arr;
        }
        logger->addTimestampMessage("End of long operation");

        logger->log() << "Hello, world!" << std::endl;
        logger->addLastGLError(__FILE__, __LINE__, "glGetError");

        return 0;
}
