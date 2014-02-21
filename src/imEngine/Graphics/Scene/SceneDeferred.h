#ifndef SCENEDEFERRED_H
#define SCENEDEFERRED_H

#include "Scene.h"
#include <imEngine/Graphics/RenderTarget.h>
#include <imEngine/Graphics/ScreenAlignedQuad.h>
#include "Skybox.h"

namespace imEngine {


/** @brief Сцена, поддерживающая отложенное освещение
 */
class SceneDeferred : public Scene {
public:
        /// Конструктор
        SceneDeferred(GraphicApplication* application);
        /// Деструктор
        ~SceneDeferred();

        /// Возвращает объект, который находится в позиции x,y в координатах окна
        Polygonal*      pickObject(int x, int y);

        /// Возвращает gbuffer
        RenderTarget*   gbuffer();

        /// Рендерит сцену
        void            render();
        /// Обработка события
        void            windowResizeEvent(int w, int h);
        void            mousePressEvent(int x, int y, char button);
        void            mouseMoveEvent(int oldX, int oldY, int newX, int newY);
        void            mouseReleaseEvent(int x, int y, char button);

public:
        void            initGBuffer();
        void            initLBuffer();

private:
        RenderTarget            m_gbuffer;
        RenderTarget            m_lbuffer;
        ScreenAlignedQuad       m_quad;
        SkyboxPtr               m_skybox;


        Polygonal*              m_pickedObject;
};


} //namespace imEngine

#endif // SCENEDEFERRED_H
