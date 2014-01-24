#ifndef GRAPHICAPPLICATION_H
#define GRAPHICAPPLICATION_H

#include "BaseApplication.h"
#include <imEngine/Graphics/GUI/FpsViewer.h>
#include <imEngine/Graphics/GAPI/GAPI.h>

namespace imEngine {

/** @brief Графическое OpenGL приложение.
 *
 *  Подраземенвается, что классы приложений ниже в иерархии будут работать с графикой, GUI,
 *  добавлять объекты на сцену.
 */
class GraphicApplication : public BaseApplication {
public:
        /// Возвращает указатель на GUI-менеджер
        GuiManager*     gui() const;

protected:
        /// Стандартный цикл приложения
        void            initialize();
        void            update();
        void            render();
        void            destroy();

        /// Обработка событий
        void            mouseMoveEvent(int oldX, int oldY, int newX, int newY);
        void            mousePressEvent(int x, int y, char button);
        void            mouseReleaseEvent(int x, int y, char button);
        void            keyPressEvent(int key);
        void            keyReleaseEvent(int key);
        void            windowResizeEvent(int x, int y);

private:
        GuiManager*     m_gui;
        FpsViewer*      m_fps;

};

} //namespace imEngine

#endif // GRAPHICAPPLICATION_H
