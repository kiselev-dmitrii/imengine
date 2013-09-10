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
        BaseApplication();
};

} //namespace imCore

#endif // BASEAPPLICATION_H
