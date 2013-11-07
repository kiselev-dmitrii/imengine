#include "Mouse.h"
#include "Window.h"
#include <SDL2/SDL.h>
#undef main

namespace imEngine {

Mouse::Mouse(Window* window) :
        m_rawWindow(window->rawWindow())
{ }

IVec2 Mouse::position() {
        IVec2 result;
        SDL_GetMouseState(&result.x, &result.y);
        return result;
}

void Mouse::setPosition(const IVec2 &position) {
        SDL_WarpMouseInWindow(m_rawWindow, position.x, position.y);
}

void Mouse::setPosition(int x, int y) {
        SDL_WarpMouseInWindow(m_rawWindow, x, y);
}

void Mouse::setVisible(bool visible) {
        SDL_ShowCursor(visible);
}

bool Mouse::isVisible() {
        return SDL_ShowCursor(-1);
}

bool Mouse::isButtonPressed(MouseButton::Enum button) {
        return SDL_GetMouseState(NULL, NULL) & button;
}


} //namespace imEngine
