#include "ProfileContext.h"
#include "Debug.h"
#include <limits>
#include <fstream>
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

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

ProfileContext::ProfileContext() { }

ProfileContext::~ProfileContext() {
        saveStatistics();
}

ProfileContext* ProfileContext::instance() {
        static ProfileContext instance;
        return &instance;
}

void ProfileContext::startProfiler(const String &name) {
        if (m_profilers.find(name) == m_profilers.end()) m_profilers.insert(make_pair(name, Profiler(name)));
        m_profilers.find(name)->second.start();
}

void ProfileContext::endProfiler(const String& name) {
        IM_ASSERT(m_profilers.find(name) != m_profilers.end());
        m_profilers.find(name)->second.end();
}

Profiler* ProfileContext::profiler(const String &name) {
        auto found = m_profilers.find(name);
        if (found == m_profilers.end()) return nullptr;
        else return &(found->second);
}

void ProfileContext::saveStatistics() {
        std::ofstream statistics;
        statistics.open("statistics.txt");

        for (auto &profiler : m_profilers) {
                statistics << "Statistic for " << profiler.second.name() << ": " << std::endl;
                statistics << "\tAverage time:\t " << profiler.second.averageDuration() << " ticks" << std::endl;
                statistics << "\tMax time:\t " << profiler.second.maxDuration() << " tics" << std::endl;
                statistics << "\tMin time:\t " << profiler.second.minDuration() << " tics" << std::endl;
                statistics << "\tTotal time:\t " << profiler.second.totalDuration() << " tics" << std::endl;
                statistics << "\tCount samples:\t " << profiler.second.countSamples() << std::endl;
                statistics << std::endl;
        }

        statistics.close();
}

}
