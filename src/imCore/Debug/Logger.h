#ifndef LOGGER_H
#define LOGGER_H

#include "../Utils/Types.h"
#include "../Utils/Uncopyable.h"
#include "../System/Timer.h"
#include <fstream>
#include <iostream>

namespace imCore {

/** @brief Синглтон для логирования в файл либо в консоль
 *
 *  По умолчанию, лог ведется в файл log.txt
 *  Существует возможность переключить вывод в рантайме с
 *  помощью метода setLogToConsole
 */
class Logger : public Uncopyable {
public:
        /// Точка доступа
        static Logger*  instance();
        /// Деструктор
                        ~Logger();

        /// Устанавливает вывод в консоль вместо файла
        void            setConsoleOutput(bool enable);

        /// Записывает в лог сообщение message
        void            addMessage(const String& message, ...);
        /// Записывает в лог сообщение message с временным штампом
        void            addTimestampMessage(const String& message, ...);
        /// Записывает в лог последнее сообщение об OpenGL ошибке если оно есть
        void            addLastGLError(const String& file, int line, const String& function);
        /// Возвращает ссылку на поток
        std::ostream&   log();

private:
        /// Закрытый конструктор
        Logger();

private:
        std::ostream*   m_log;
        std::ofstream   m_logFileStream;
        const String    m_logFilename = "log.txt";
        static Timer    m_timer;
};

} //namespace imCore

#endif // LOGGER_H
