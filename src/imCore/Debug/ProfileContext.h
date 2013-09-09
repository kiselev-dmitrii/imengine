#ifndef PROFILECONTEXT_H
#define PROFILECONTEXT_H

#include <unordered_map>
#include "../Utils/Types.h"
#include "../Utils/Uncopyable.h"
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


///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////


/** @brief Класс-синглтон, обеспечивающий работу с профилировщиками.
 *
 *  Использование данного класса наиболее удобно через макросы
 *  IM_START_PROFILER и IM_END_PROFILE, определенных в Debug.h.
 *  По завершению программы, профайлер автоматически сохранит результаты з
 *  замеров в файле statistics.txt в удобной форме.
 */
class ProfileContext : private Uncopyable {
public:
        /// Точка доступа
        static ProfileContext* instance();
        /// Деструктор
        ~ProfileContext();

        /// Начать секцию для профилирования
        void            startProfiler(const String& name);
        /// Закончить секцию для профилирования
        void            endProfiler(const String& name);

        /// Получить i-тый профайл
        Profiler*       profiler(uint i);
        /// Получить профайлер с именем name. Возвращает nullptr, если не найдено
        Profiler*       profiler(const String& name);

private:
        /// Конструктор
        ProfileContext();
        /// Сохраняет статистику профилирования в файл
        void            saveStatistics();

private:
        std::unordered_map<String, Profiler>    m_profilers;

};


} //namespace imCore

#endif // PROFILECONTEXT_H
