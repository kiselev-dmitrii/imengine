#include <imCore/Debug/Logger.h>

using namespace imCore;

int main() {
        Logger* logger = Logger::instance();
        logger->addOutMessage("Out to file%u", 1);
        logger->addLogMessage("Log to file%u", 1);
        logger->addOutMessage("Out to file%u", 2);
        logger->addLogMessage("Log to file%u", 2);

        logger->setLogToConsole(true);
        logger->addLogMessage("Log to %s %u", "console", 1);
        logger->addOutMessage("Out to file3");

        logger->setOutToConsole(true);
        logger->addLogMessage("Log to console2");
        logger->addOutMessage("Out to console1");
        logger->out() << "Just Test" << std::endl;

        logger->setOutToConsole(false);
        logger->setLogToConsole(false);
        logger->addLogMessage("Log to file3");
        logger->addOutMessage("Out to file4");

        logger->setLogToConsole(true);

        logger->log() << "MASSIVE OPERATIONS:" << std::endl;
        logger->addLogMessage("Start of long operation");
        for (long long i=0; i < 34*100500; ++i) {
                int* arr = new int [100500];
                delete arr;
        }
        logger->addLogMessage("End of long operation");


        return 0;
}
