#ifndef WINDOW_H
#define WINDOW_H

#include "../Math/Common.h"
#include "../Utils/Types.h"
#include <GLFW/glfw3.h>

namespace imCore {

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

        /// Создает и отображает окно
        bool            open();

        /// Вовзвращает заголовок окна
        String          title();
        /// Устанавливает заголовок окна
        void            setTitle(const String& title);

        /// Возвращает позицию окна
        ivec2           position();
        /// Устанавливает позицию окна
        void            setPosition(const ivec2& position);
        void            setPosition(int x, int y);

        /// Возвращает размеры окна
        ivec2           size();
        /// Устанавливает размеры окна
        void            setSize(const ivec2& size);
        void            setSize(int width, int height);

        /// Определяет, запущено ли окно в полный экран
        bool            isFullscreen();
        /// Устанавливает полноэкранный режим
        void            setFullscreen(bool isFullscreen);

        /// Возвращает координаты центра окна
        ivec2           center();

        /// Возвращает ссылку на GLFW окно
        GLFWwindow*     glfwWindow();

private:
        GLFWwindow*     m_window;
        String          m_title;
        ivec2           m_position;
        ivec2           m_size;
        ivec2           m_isFullscreen;

};

} //namespace imCore

#endif // WINDOW_H
