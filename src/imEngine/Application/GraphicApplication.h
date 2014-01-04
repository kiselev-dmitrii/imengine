#ifndef GRAPHICAPPLICATION_H
#define GRAPHICAPPLICATION_H

#include "BaseApplication.h"
#include <imEngine/Graphics/GUI/Text.h>
#include <imEngine/Graphics/GAPI/GAPI.h>

namespace imEngine {

/** @brief Графическое OpenGL приложение.
 *
 *  Подраземенвается, что классы приложений ниже в иерархии будут работать с графикой, GUI,
 *  добавлять объекты на сцену.
 */
class GraphicApplication : public BaseApplication {
public:
        /// Возвращает текущий FPS
        uint            fps() const;

protected:
        /// Инициализирует графическое приложение. Создание менеджеров, настройка OpenGL.
        void            initialize();
        /// Выполняется обновление состояния графического приложения
        void            update();
        /// Рендер вспомогательной информации
        void            render();
        /// Действия по закрытию графического приложения.
        void            destroy();

        /// Обработка стандартных клавиш (развернуть, и т.д)
        void            keyPressEvent(int key);

private:
        void            initFPS();
        void            updateFPS();

private:
        /// Данные про FPS
        Text*           m_textFps;
        uint            m_fps;
        double          m_oldTime;

};

} //namespace imEngine

#endif // GRAPHICAPPLICATION_H
