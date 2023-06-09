#ifndef MOUSE_H
#define MOUSE_H

#include "../Math/Common.h"
#include <SDL2/SDL.h>

namespace imEngine {
class Window;

/** @brief Кнопки мыши
 */
namespace MouseButton {
enum Enum {
        LEFT = SDL_BUTTON_LEFT,
        MIDDLE = SDL_BUTTON_MIDDLE,
        RIGHT = SDL_BUTTON_RIGHT
};
}

/** @brief Класс мышки, основанного на библиотеке SDL
 */
class Mouse {
public:
        /// Конструктор. Мышь привязывается к окну window.
        Mouse(Window* window);

        /// Возвращает позицию мышки в оконных координатах
        IVec2   position();
        /// Устанавливает позицию мышки в оконных координатах
        void    setPosition(const IVec2& position);
        /// Аналогично
        void    setPosition(int x, int y);

        /// Устанавливает видимость курсора
        void    setVisible(bool visible);
        /// Определяет видим ли курсор
        bool    isVisible();

        /// Определяет, нажата ли кнопка мыши button
        bool    isButtonPressed(MouseButton::Enum button);

private:
        SDL_Window*     m_rawWindow;
};

} //namespace imEngine

#endif // MOUSE_H
