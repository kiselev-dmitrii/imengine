#ifndef SIGNAL_H
#define SIGNAL_H

#include <functional>
#include <list>

namespace imEngine {


/** @brief Шаблонный класс, представляющий собой сигнал.
 *  Сигналы можно соединять с другими функциями. Тогда при вызове сингала будут
 *  автоматически вызваны присоединенные фунции
 */
template <typename... ArgsTypes>
class Signal {
public:
        typedef std::function<void(ArgsTypes...)>  Function;

        void            operator +=(Function func)                              { m_functions.push_back(func); }
        void            operator ()(ArgsTypes... args) const                    { for(Function func: m_functions) func(args...); }

private:
        typedef std::list<Function>     FunctionList;
        FunctionList    m_functions;
};


} //namesapce imEngine

#endif // SIGNAL_H
