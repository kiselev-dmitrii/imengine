#include "Logger.h"
#include <cstdio>
#include <cstdarg>
#include "GLErrorMonitor.h"
#include "Debug.h"

namespace imCore {

Timer Logger::m_timer;

Logger::Logger() {
        m_logFileStream.open(m_logFilename);
        m_log = &m_logFileStream;
}

Logger::~Logger() {
        m_logFileStream.close();
}

Logger* Logger::instance() {
        static Logger instance;
        return &instance;
}

void Logger::setConsoleOutput(bool enable) {
        if (enable) m_log = &std::clog;
        else m_log = &m_logFileStream;
}

void Logger::addMessage(const String &message, ...) {
        // Распаковка аргументов
        va_list args;
        va_start(args, message);
        char buffer[512];
        vsnprintf(buffer, 512, message.c_str(), args);
        va_end(args);

        log() << buffer << std::endl;
}

void Logger::addTimestampMessage(const String &message, ...) {
        // Распаковка аргументов
        va_list args;
        va_start(args, message);
        char buffer[512];
        vsnprintf(buffer, 512, message.c_str(), args);
        va_end(args);

        log() << '[' << m_timer.timestamp() << ']' << '\t' << buffer << std::endl;
}

void Logger::addLastGLError(const String &file, int line, const String &function) {
        GLenum error = GLErrorMonitor::lastError();
        if (error == GL_NO_ERROR) return;
        log() << '[' << m_timer.timestamp() << ']' << '\t';
        log() << "Function " << function << " return " << GLErrorMonitor::glEnumToString(error) << "; file: " <<  file << "; line: " << line << std::endl;
}

std::ostream& Logger::log() {
        return *m_log;
}

} //namespace imCore
