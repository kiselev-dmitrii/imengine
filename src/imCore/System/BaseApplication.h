#ifndef BASEAPPLICATION_H
#define BASEAPPLICATION_H

namespace imCore {

/** @brief Данный класс служит для ициниализации окна,
 *  основных библиотек, вроде GLEW, GLFW, OpenAL, DevIL и прочих.
 *
 *  От данного класса будут наследоваться классы вроде GuiApplication и прочих,
 *  в которых содержаться более высокоуровневые компоненты
 */
class BaseApplication {
public:
        /// Запускает приложение
        int     exec();

protected:
        virtual void    initialize() = 0;
        virtual void    update() = 0;
        virtual void    render() = 0;
        virtual void    destroy() = 0;

        /// События клавиатуры
        virtual void    keyPressEvent()         {}
        virtual void    keyReleaseEvent()       {}

        /// События мыши
        virtual void    mouseWheelEvent()       {}
        virtual void    mousePressEvent()       {}
        virtual void    mouseReleaseEvent()     {}

private:
        /// Инициализация библиотек, создание базового окна
        bool    init();
        /// Выполнение главного цикла
        void    loop();
        /// Завершение приложения
        void    quit();
};

} //namespace imCore

#endif // BASEAPPLICATION_H
