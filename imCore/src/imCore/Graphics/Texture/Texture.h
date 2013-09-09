#ifndef TEXTURE_H
#define TEXTURE_H

namespace imCore {

/** @brief Класс Texture - основной базовый класс для работы с текстурами. НЕРЕАЛИЗОВАН
 */
class Texture {
public:
        /// Конструктор
        explicit        Texture();
        /// Виртуальный деструктор
        virtual         ~Texture();
};

} //namespace imCore

#endif // TEXTURE_H
