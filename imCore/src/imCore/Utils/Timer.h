#ifndef TIMER_H
#define TIMER_H

#include <ctime>
#include "Types.h"

namespace imCore {

/** @brief Класс предназначен для подсчета времени/тактов
 *  просшедших с момента создания класса или вызова метода reset
 */
class Timer {
public:
        /// Конструктор
        explicit        Timer();

        /// Сброс таймера
        void            reset();

        /// Время в формате строки hours:mins:secs:clocks
        String          timestamp();
        /// Возвращает прошедшее время в часах
        ULong           hours();
        /// Возвращает прошедшее время в минутах
        ULong           minutes();
        /// Возвращает прошедшее время в секундах
        ULong           seconds();
        /// Возвращает прошедшее время в тактах
        ULong           ticks();

private:
        clock_t m_resetTime;
};

} //namespace imCore

#endif // TIMER_H
