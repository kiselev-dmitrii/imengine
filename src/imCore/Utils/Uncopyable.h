#ifndef UNCOPYABLE_H
#define UNCOPYABLE_H

namespace imCore {

/** @brief Базовый класс, служащий для запрета копирования объекта
 */
class Uncopyable {
protected:
        /// Конструктор
        Uncopyable()                                    {}
        /// Деструктор
        ~Uncopyable()                                   {}

private:
        /// Закрытый конструктор копирования
        Uncopyable(const Uncopyable& obj);
        /// Закрытый оператор присваивания
        Uncopyable& operator=(const Uncopyable& op2);

};

} //namespace imCore

#endif // UNCOPYABLE_H
