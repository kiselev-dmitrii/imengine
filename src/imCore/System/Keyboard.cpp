#include "Keyboard.h"

namespace imCore {

bool Keyboard::isKeyPressed(int key) {
        const Uint8* state = SDL_GetKeyboardState(NULL);
        return state[SDL_GetScancodeFromKey(key)];
}

KeyboardModifiers Keyboard::modifiers() {
        return KeyboardModifiers(SDL_GetModState());
}

} //namespace imCore
