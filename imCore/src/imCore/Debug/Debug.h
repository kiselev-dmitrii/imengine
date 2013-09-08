#ifndef DEBUG_H
#define DEBUG_H

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
                std::cout << text << " in " << __FUNCTION__ << ", line " << __LINE__ << ", file " << __FILE__ << std::endl

        /// Вывод сообщения, что функция не реализована
        #define IM_TODO\
                std::cout << "Function " << __FUNCTION__ << " is not implemented in " << __FILE__ << " in line " << __LINE__ << std::endl

        /// Утверждения времени выполнения
        #define IM_ASSERT(expression)\
                assert(expression)

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


#endif // DEBUG_H
