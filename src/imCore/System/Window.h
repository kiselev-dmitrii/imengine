#ifndef WINDOW_H
#define WINDOW_H

#include "../Math/Common.h"
#include "../Utils/Types.h"
#include <SDL2/SDL.h>
#undef main

namespace imCore {

/** @brief Класс окна, основанного на библиотеке SDL.
 *
 *  Перед вызовом метода create, необходимо чтобы библиотека SDL
 *  была проинициализирована. Метод create непосредственно создает окно.
 *  До вызова create, вызовы методов кэшируются и применяются при создании окна.
 *
 *  Метод destroy необходимо вызывать перед завершением SDL. Собственно поэтому он
 *  и введен, чтобы управлять порядком удаления
 */
class Window {
public:
        /// Конструктор
        Window();
        /// Деструктор
        ~Window();

        /// Создает окно и контекст
        bool            create();
        /// Удаляет окно и уничтожает контекст
        void            destroy();

        /// Определяет, видимо ли окно
        bool            isVisible();
        /// Отображает окно
        void            show();
        /// Скрывает окно
        void            hide();

        /// Вовзвращает заголовок окна
        String          title();
        /// Устанавливает заголовок окна
        void            setTitle(const String& title);

        /// Возвращает позицию окна.
        IVec2           position();
        /// Устанавливает позицию окна
        void            setPosition(const IVec2& position);
        void            setPosition(int x, int y);

        /// Возвращает размеры окна. Если isFullscreen == true, то возвращает разрешение экрана
        IVec2           size();
        /// Устанавливает размеры окна
        void            setSize(const IVec2& size);
        void            setSize(int width, int height);

        /// Определяет, запущено ли окно в полный экран
        bool            isFullscreen();
        /// Устанавливает полноэкранный режим
        void            setFullscreen(bool isFullscreen);

        /// Возвращает координаты центра окна.
        IVec2           center();

        /// Возвращает ссылку на GLFW окно
        SDL_Window*     rawWindow();

private:
        /// Создает окно
        bool            createWindow();
        /// Создает контекст
        bool            createContext();

private:
        SDL_Window*     m_window;
        SDL_GLContext   m_context;

        String          m_title;
        IVec2           m_position;
        IVec2           m_size;
        IVec2           m_prevSize;     //необходим для запоминания размера перед входов в Fullscreen
        bool            m_isVisible;
        bool            m_isFullscreen;

};

} //namespace imCore

#endif // WINDOW_H
