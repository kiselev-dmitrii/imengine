#ifndef GRAPHICAPPLICATION_H
#define GRAPHICAPPLICATION_H

#include "BaseApplication.h"

namespace imEngine {

/** @brief Графическое OpenGL приложение.
 *
 *  Подраземенвается, что классы приложений ниже в иерархии будут работать с графикой, GUI,
 *  добавлять объекты на сцену.
 *  @todo
 *      - Реализовать
 */
class GraphicApplication : public BaseApplication {
protected:
        /// Инициализирует графическое приложение. Создание менеджеров, настройка OpenGL.
        void            initialize();
        /// Выполняется обновление состояния графического приложения
        void            update();
        /// Действия выполняющиеся перед рендером
        void            render();
        /// Действия по закрытию графического приложения.
        void            destroy();

        /// Обработка стандартных клавиш (развернуть, и т.д)
        void            keyPressEvent(int key);

private:
        /*
        ResourceManager m_resourceManager;
        SceneManager    m_sceneManager;
        GuiManager      m_guiManager;
        */
};

} //namespace imEngine

#endif // GRAPHICAPPLICATION_H
