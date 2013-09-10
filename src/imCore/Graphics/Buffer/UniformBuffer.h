#ifndef UNIFORMBUFFER_H
#define UNIFORMBUFFER_H

namespace imCore {

/** @brief Класс представляет собой хранилище данных для передачи в шейдер.
 *
 *  С помощью данного класса можно упаковать крупный объем данных и
 *  осуществлять передачу целого пакета в шейдер. Таким образом можно повысить производительность.
 */
class UniformBuffer {
public:
        UniformBuffer();
};

} //namespace imCore

#endif // UNIFORMBUFFER_H
