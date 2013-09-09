#include "Profiler.h"
#include <limits>
#include <GL/glew.h>

namespace imCore {

Profiler::Profiler(const String& name) :
        m_name(name),
        m_maxDuration(0),
        m_minDuration(std::numeric_limits<ulong>::max()),
        m_totalDuration(0),
        m_countSamples(0)
{ }

String Profiler::name() {
        return m_name;
}

ulong Profiler::totalDuration() {
        return m_totalDuration;
}

ulong Profiler::maxDuration() {
        return m_maxDuration;
}

ulong Profiler::minDuration() {
        return m_minDuration;
}

double Profiler::averageDuration() {
        if (m_countSamples == 0) return 0;
        else return double(m_totalDuration) / m_countSamples;
}

uint Profiler::countSamples() {
        return m_countSamples;
}

void Profiler::start() {
        // Завершает все GL вызовы, т.е ожидает пока они не выполнятся для корректного измерения след. кода
        glFinish();
        m_timer.reset();
}

void Profiler::end() {
        // Дожидаемся пока выполнятся GL вызовы
        glFinish();

        // Замер
        ulong curTime = m_timer.ticks();

        // Обновляем показатели
        if (curTime > m_maxDuration) m_maxDuration = curTime;
        if (curTime < m_minDuration) m_minDuration = curTime;
        m_totalDuration += curTime;
        ++m_countSamples;
}

}
