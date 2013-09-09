#ifndef PROFILERMANAGER_H
#define PROFILERMANAGER_H

#include "Profiler.h"
#include <unordered_map>
#include "../Utils/Uncopyable.h"
#include "../Utils/Types.h"

namespace imCore {

/** @brief Менеджер профайлов (синглтон).
 *
 *  Использование данного менеджера наиболее удобно через макросы
 *  IM_START_PROFILER и IM_END_PROFILE, определенных в Debug.h.
 *  По завершению программы, профайлер автоматически сохранит результаты з
 *  замеров в файле statistics.txt в удобной форме.
 */
class ProfilerManager : private Uncopyable {
public:
        /// Точка входа
        static ProfilerManager* instance();
        /// Деструктор
                                ~ProfilerManager();

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
                        ProfilerManager();

        /// Сохраняет статистику профилирования в файл
        void            saveStatistics();

private:
        std::unordered_map<String, Profiler>    m_profilers;
};

} //namespace imCore

#endif // PROFILERMANAGER_H
