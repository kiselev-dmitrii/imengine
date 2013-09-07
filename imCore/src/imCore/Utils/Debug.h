#ifndef DEBUG_H
#define DEBUG_H

#include <iostream>
#include <assert.h>

/// Вывод во время дебага
#define IM_SHOW(variable)       std::cout << #variable" = " << variable << std::endl
#define IM_DEBUG(text)          std::cout << text << std::endl
#define IM_TRACE(text)          std::cout << text << " in " << __FUNCTION__ << ", line " << __LINE__ << ", file " << __FILE__ << std::endl
#define IM_TODO                 std::cout << "Function " << __FUNCTION__ << " is not implemented in " << __FILE__ << " in line " << __LINE__ << std::endl

/// Утверждения времени выполнения
#define IM_ASSERT(expression)   assert(expression)

#endif // DEBUG_H
