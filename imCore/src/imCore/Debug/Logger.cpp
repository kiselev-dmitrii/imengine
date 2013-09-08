#include "Logger.h"
#include <cstdio>
#include <cstdarg>

namespace imCore {

Timer Logger::m_timer;

Logger::Logger() {
        m_outFileStream.open(m_outFilename);
        m_logFileStream.open(m_logFilename);

        m_out = &m_outFileStream;
        m_log = &m_logFileStream;
}

Logger::~Logger() {
        m_outFileStream.close();
        m_logFileStream.close();
}

Logger* Logger::instance() {
        static Logger instance;
        return &instance;
}

void Logger::setLogToConsole(bool enable) {
        if (enable) m_log = &std::clog;
        else m_log = &m_logFileStream;
}

void Logger::setOutToConsole(bool enable) {
        if (enable) m_out = &std::cout;
        else m_out = &m_outFileStream;
}

void Logger::outMessage(const String &message, ...) {
        // Распаковка аргументов
        va_list args;
        va_start(args, message);
        char buffer[512];
        vsnprintf(buffer, 512, message.c_str(), args);
        va_end(args);

        writeMessage(*m_out, buffer);
}

void Logger::logMessage(const String &message, ...) {
        // Распаковка аргументов
        va_list args;
        va_start(args, message);
        char buffer[512];
        vsnprintf(buffer, 512, message.c_str(), args);
        va_end(args);

        writeMessage(*m_log, buffer);
}

void Logger::writeMessage(std::ostream &stream, const String &message) {
        stream << '[' << m_timer.timestamp() << ']' << '\t' << message << std::endl;
}


} //namespace imCore
