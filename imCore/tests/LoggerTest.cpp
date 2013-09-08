#include <imCore/Debug/Logger.h>

using namespace imCore;

int main() {
        Logger* logger = Logger::instance();
        logger->outMessage("Out to file%u", 1);
        logger->logMessage("Log to file%u", 1);
        logger->outMessage("Out to file%u", 2);
        logger->logMessage("Log to file%u", 2);

        logger->setLogToConsole(true);
        logger->logMessage("Log to %s %u", "console", 1);
        logger->outMessage("Out to file3");

        logger->setOutToConsole(true);
        logger->logMessage("Log to console2");
        logger->outMessage("Out to console1");
        logger->out() << "Just Test" << std::endl;

        logger->setOutToConsole(false);
        logger->setLogToConsole(false);
        logger->logMessage("Log to file3");
        logger->outMessage("Out to file4");

        logger->setLogToConsole(true);

        logger->log() << "MASSIVE OPERATIONS:" << std::endl;
        logger->logMessage("Start of long operation");
        for (long long i=0; i < 34*100500; ++i) {
                int* arr = new int [100500];
                delete arr;
        }
        logger->logMessage("End of long operation");


        return 0;
}
