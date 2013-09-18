#ifndef DEBUG_H
#define DEBUG_H

#include "StreamUtils.h"
#include "Types.h"

using namespace imCore::StreamUtils;

namespace imCore {

/** @brief Содержит вспомогательные функции, для дебага
 */
namespace Debug {
        /// Возвращает строку вида [hour:minute:second::millisecond]
        String          currentTimeStamp();

        /// Определяет есть ли на данный момент ошибки OpenGL, и если есть выводит сообщение в лог
        void            checkGlErrors(const String& function, const String& file, uint line, const String& call);

}


/** @brief Макросы для дебага
 */
#ifdef IM_DEBUG
        /// Вывод вывод переменной
        #define IM_VAR(variable)\
                std::cout << #variable" = " << variable << std::endl

        /// Вывод текста
        #define IM_PRINT(text)\
                std::cout << text << std::endl

        /// Трасировка с сообщением
        #define IM_TRACE(text)\
                std::clog << imCore::Debug::currentTimeStamp() << " TRACE: Function " <<  __FUNCTION__ << "(...) , File: " << __FILE__ << ", Line: " << __LINE__ << ", Text: " << text << std::endl;

        /// Вывод сообщения, что функция не реализована
        #define IM_TODO\
                std::clog << imCore::Debug::currentTimeStamp() << " TODO: Function " <<  __FUNCTION__ << "(...) , Fine: " << __FILE__ << ", Line: " << __LINE__ << std::endl;

        /// Вывод ошибки в лог
        #define IM_ERROR(text)\
                std::clog << imCore::Debug::currentTimeStamp() << " ERROR: Function " <<  __FUNCTION__ << "(...) , Fine: " << __FILE__ << ", Line: " << __LINE__ << ", Cause: " << text << std::endl;

        /// Просто сообщение в лог
        #define IM_LOG(text)\
                std::clog << imCore::Debug::currentTimeStamp() << " LOG: " << text << std::endl;

        /// Утверждения времени выполнения
        #define IM_ASSERT(expression)\
                assert(expression)

        /// Вызов OpenGL функции с логом
        #define IM_GLCALL(call)\
                call;\
                imCore::Debug::checkGlErrors(__FUNCTION__, __FILE__, __LINE__, #call)

#else
        #define IM_VAR(variable)
        #define IM_PRINT(text)
        #define IM_TRACE(text)
        #define IM_TODO
        #define IM_ERROR(text)
        #define IM_ASSERT(expression)
        #define IM_GLCALL(call) call
#endif


} //namespace imCore

#endif // DEBUG_H