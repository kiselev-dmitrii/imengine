#ifndef PROFILER_H
#define PROFILER_H

#include "../Utils/Types.h"
#include "../System/Timer.h"

namespace imCore {

/** @brief Профайлер. Используется для замера времени выполнения некоторых функций.
 *
 *  Профайлер используется в целях дебага. Используется в составе ProfileManager.
 *  Наиболее удобно использовать профайлер через макросы IM_START_PROFILER и IM_END_PROFILER
 *  определенных в файле Debug.h
 */
class Profiler {
public:
        /// Создает профайлер
        Profiler(const String& name);

        /// Имя профайла
        String  name();

        /// Общее время работы секции в тактах
        ulong   totalDuration();

        /// Максимальное время работы секции
        ulong   maxDuration();

        /// Минимальное время работы секции
        ulong   minDuration();

        /// Среднее время работы секции
        double  averageDuration();

        /// Количество запусков данного профайла
        uint    countSamples();

        /// Начать замер
        void    start();
        /// Закончить замер
        void    end();

private:
        String  m_name;
        ulong   m_maxDuration;
        ulong   m_minDuration;
        ulong   m_totalDuration;
        ulong   m_countSamples;
        Timer   m_timer;
};

} //namespace imCore

#endif // PROFILER_H
