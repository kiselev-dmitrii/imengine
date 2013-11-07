#ifndef FONT_H
#define FONT_H

namespace imEngine {

/** @brief Шрифт. Представляет собой обертку для библиотеки Freetype
 *  @todo
 *      - Анилизировать возможность создания шрифта из обычных текстур (не реализуя класса Image)
 *      - Реализовать методы загрузки из ttf
 *      - Методы получения текстуры шрифта
 *      - Метод получения текстурных координат для каждого глифа
 */
class Font {
public:
        Font();
};

} //namespace imEngine

#endif // FONT_H
