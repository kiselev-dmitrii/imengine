#ifndef BASEAPPLICATION_H
#define BASEAPPLICATION_H

#include <imEngine/System/Window.h>
#include <chrono>

using std::chrono::time_point;
using std::chrono::system_clock;

namespace imEngine {

/** @brief Данный класс служит для ициниализации окна,
 *  основных библиотек, вроде GLEW, GLFW, OpenAL, DevIL и прочих.
 *
 *  От данного класса будут наследоваться классы вроде GuiApplication и прочих,
 *  в которых содержаться более высокоуровневые компоненты
 */
class BaseApplication {
public:
        /// Запускает приложение
        int             exec();

        /// Возвращает указатель на главное окно приложения
        Window*         mainWindow();

        /// Возвращает время работы программы
        double          currentTime();

protected:
        /// Выполняется при инициализации приложения
        virtual void    initialize() = 0;
        /// Данный метод нужно переопределить для обновления состояния объектов
        virtual void    update() = 0;
        /// Данный метод необходимо переопределить для отрисовки объектов
        virtual void    render() = 0;
        /// Выполняется при уничтожении приложения
        virtual void    destroy() = 0;

        /// Вызывается при нажатии на клавишу
        virtual void    keyPressEvent(int key)                                  {}
        /// Вызывается при отжатии клавиши
        virtual void    keyReleaseEvent(int key)                                {}

        /// Вызывается при нажатии на кнопку мыши
        virtual void    mousePressEvent(int x, int y, char button)              {}
        /// Вызывается при отжатии кнопки мыши
        virtual void    mouseReleaseEvent(int x, int y, char button)            {}
        /// Вызывается при изменении позиции колесика мыши.
        /// Вертикальное и горизонтальное приращение записывается в deltaVertical и deltaHorizontal
        virtual void    mouseWheelEvent(int deltaVertical, int deltaHorizontal) {}
        /// Вызывается при изменении позиции курсора мыши
        virtual void    mouseMoveEvent(int oldX, int oldY, int newX, int newY)  {}

        /// Вызывается при изменении размеров окна
        virtual void    windowResizeEvent(int x, int y)                         {}
        /// Вызывается минимизации окна
        virtual void    windowMinimizeEvent()                                   {}

private:
        /// Инициализация библиотек, создание базового окна
        bool    init();
        /// Выполнение главного цикла
        void    loop();
        /// Завершение приложения
        void    quit();

        bool    initSDL();
        bool    createMainWindow();
        bool    initGLEW();
        bool    initDevIL();
        void    initTimer();

private:
        Window                          m_mainWindow;
        time_point<system_clock>        m_startTime;

};

} //namespace imEngine

#endif // BASEAPPLICATION_H
