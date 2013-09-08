#ifndef LOGGER_H
#define LOGGER_H

#include "../Utils/Types.h"
#include "../Utils/Uncopyable.h"
#include "../Utils/Timer.h"
#include <fstream>
#include <iostream>

namespace imCore {

/** @brief Синглтон для логирования в файл либо в консоль
 *
 *  По умолчанию, вывод ведется в файлы log.txt и out.txt.
 *  Существует возможность переключить вывод в рантайме с
 *  помощью метода set*ToConsole
 */
class Logger : public Uncopyable {
public:
        /// Точка доступа
        static Logger*  instance();
        /// Деструктор
                        ~Logger();

        /// Устанавливает вывод в консоль вместо файла
        void            setOutToConsole(bool enable);
        /// Устанавливает лог в консоль вместо файла
        void            setLogToConsole(bool enable);

        /// Записывает в выход сообщение message с временным штампом
        void            outMessage(const String& message, ...);
        /// Записывает в лог сообщение message с временным штампом
        void            logMessage(const String& message, ...);

private:
        /// Закрытый конструктор
        Logger();
        /// Помещает сообщение в некоторый поток
        void            writeMessage(std::ostream& stream, const String& message);

private:
        std::ostream*   m_out;
        std::ostream*   m_log;

        std::ofstream   m_outFileStream;
        std::ofstream   m_logFileStream;
        const String    m_logFilename = "log.txt";
        const String    m_outFilename = "out.txt";

        static Timer    m_timer;
};

} //namespace imCore

#endif // LOGGER_H
