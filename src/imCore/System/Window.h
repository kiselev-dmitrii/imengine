#ifndef WINDOW_H
#define WINDOW_H

#include "../Math/Common.h"
#include "../Utils/Types.h"
#include <GLFW/glfw3.h>

namespace imCore {
/** @brief Структура для хранения информации о контексте OpenGL
 */
struct Context {
        int     majorVersion;
        int     minorVersion;
        int     profile;
        int     samples;
};


/** @brief Класс окна, основанного на библиотеке GLFW3.
 *
 *  Для работы с классом Window необходимо, чтобы GLFW3 был
 *  проинициализирован.
 */
class Window {
public:
        /// Конструктор
        Window();
        /// Деструктор
        ~Window();

        /// Отображает окно
        void            show();
        /// Скрывает окно
        void            hide();

        /// Вовзвращает заголовок окна
        String          title();
        /// Устанавливает заголовок окна
        void            setTitle(const String& title);

        /// Возвращает позицию окна
        ivec2           position();
        /// Устанавливает позицию окна
        void            setPosition(const ivec2& position);

        /// Устанавливает полноэкранный режим
        void            setFullscreen(bool enable);
        /// Определяет, запущено ли окно в полный экран
        bool            fullscreen();

        /// Возвращает размеры окна
        ivec2           size();
        /// Устанавливает размеры окна
        void            setSize(const ivec2& size);
        /// Вовзвращает размеры фреймбуфера
        ivec2           framebufferSize();

        /// Возвращает ссылку на GLFW окно
        GLFWwindow*     rawWindow();

private:
        /// Создает окно
        void            createWindow();

private:
        GLFWwindow*     m_window;
        String          m_title;
        ivec2           m_position;
        ivec2           m_size;
        bool            m_isFullscreen;
        Context         m_context;
};

} //namespace imCore

#endif // WINDOW_H
