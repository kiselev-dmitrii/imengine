#ifndef DEBUG_H
#define DEBUG_H

#include "Logger.h"
#include "ProfilerManager.h"
#include <iostream>
#include <assert.h>

#ifdef IM_DEBUG
        /// Вывод вывод переменной
        #define IM_SHOW(variable)\
                std::cout << #variable" = " << variable << std::endl

        /// Вывод текста
        #define IM_PRINT(text)\
                std::cout << text << std::endl

        /// Трасировка с сообщением
        #define IM_TRACE(text)\
                imCore::Logger::instance()->addTimestampMessage("%s in %s, line %u, file %s", text, __FUNCTION__, __LINE__, __FILE__)

        /// Вывод сообщения, что функция не реализована
        #define IM_TODO\
                imCore::Logger::instance()->addTimestampMessage("Function %s is not imlemented in %s in line %s", __FUNCTION__, __FILE__, __LINE__)

        /// Утверждения времени выполнения
        #define IM_ASSERT(expression)\
                assert(expression)

        /// Начать профилирование секции
        #define IM_START_PROFILER(name)\
                imCore::ProfilerManager::instance()->startProfiler(name)

        /// Закончить профилирование секции
        #define IM_END_PROFILER(name)\
                imCore::ProfilerManager::instance()->endProfiler(name)


        /// Вызов OpenGL функции с логом
        #define IM_GLCALL(function)\
                function

#else
        #define IM_SHOW(variable)
        #define iM_PRINT(text)
        #define iM_TRACE(text)
        #define iM_TODO
        #define IM_ASSERT(expression)
        #define IM_GLCALL(function) function
#endif


#endif // DEBUG_H
