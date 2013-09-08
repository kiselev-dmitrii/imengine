#include "Timer.h"

namespace imCore {

Timer::Timer() {
        reset();
}

void Timer::reset() {
        m_resetTime = clock();
}

String Timer::timestamp() {
        clock_t time = clock() - m_resetTime;

        clock_t hours = time / (CLOCKS_PER_SEC * 3600);
        clock_t minutes = time / (CLOCKS_PER_SEC * 60);
        clock_t seconds = time / CLOCKS_PER_SEC;
        clock_t ticks = time % CLOCKS_PER_SEC;
        seconds -= minutes * 60;
        minutes -= hours * 60;

        String result = std::to_string(hours) + ":" +
                        std::to_string(minutes) + ":" +
                        std::to_string(seconds) + ":"+
                        std::to_string(ticks);
        return result;
}

ULong Timer::hours() {
        clock_t time = clock() - m_resetTime;
        return time / (CLOCKS_PER_SEC * 3600);
}

ULong Timer::minutes() {
        clock_t time = clock() - m_resetTime;
        return time / (CLOCKS_PER_SEC * 60);
}

ULong Timer::seconds() {
        clock_t time = clock() - m_resetTime;
        return time / CLOCKS_PER_SEC;
}

ULong Timer::ticks() {
        clock_t time = clock() - m_resetTime;
        return time;
}

} //namespace imCore
