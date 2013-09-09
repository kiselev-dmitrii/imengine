#include "ProfilerManager.h"
#include "Debug.h"
#include <fstream>

namespace imCore {

ProfilerManager::ProfilerManager() { }

ProfilerManager::~ProfilerManager() {
        saveStatistics();
}

ProfilerManager* ProfilerManager::instance() {
        static ProfilerManager instance;
        return &instance;
}

void ProfilerManager::startProfiler(const String &name) {
        if (m_profilers.find(name) == m_profilers.end()) m_profilers.insert(make_pair(name, Profiler(name)));
        m_profilers.find(name)->second.start();
}

void ProfilerManager::endProfiler(const String& name) {
        IM_ASSERT(m_profilers.find(name) != m_profilers.end());
        m_profilers.find(name)->second.end();
}

Profiler* ProfilerManager::profiler(const String &name) {
        auto found = m_profilers.find(name);
        if (found == m_profilers.end()) return nullptr;
        else return &(found->second);
}

void ProfilerManager::saveStatistics() {
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


} //namespace imCore
