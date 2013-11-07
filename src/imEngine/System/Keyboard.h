#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <SDL2/SDL.h>
#undef main

namespace imEngine {

/** @brief Модификаторы клавиатуры
 */
namespace KeyboardModifiers {
enum Enum {
        NONE = KMOD_NONE,

        LSHIFT = KMOD_LSHIFT,
        RSHIFT = KMOD_RSHIFT,
        SHIFT = KMOD_CTRL,

        LCTRL = KMOD_LCTRL,
        RCTRL = KMOD_RCTRL,
        CTRL = KMOD_CTRL,

        LALT = KMOD_LALT,
        RALT = KMOD_RALT,
        ALT = KMOD_ALT,

        LSUPER = KMOD_LGUI,
        RSUPER = KMOD_RGUI,
        SUPER = KMOD_GUI,

        NUM = KMOD_NUM,
        CAPS = KMOD_CAPS,
        MODE = KMOD_MODE,
        RESERVED = KMOD_RESERVED
};
}

/** @brief Класс для работы с клавиатурой.
 *
 *  Данный класс является оберткой для библиотеки SDL.
 */
class Keyboard {
public:
        /// Определяет нажата ли клавиша с кодом key
        bool                    isKeyPressed(int key);

        /// Возвращает битовый массив нажатых модификаторов
        KeyboardModifiers::Enum modifiers();
};

} //namespace imEngine

#endif // KEYBOARD_H
