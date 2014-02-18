#ifndef SCENEDEFERRED_H
#define SCENEDEFERRED_H

#include "Scene.h"
#include <imEngine/Graphics/RenderTarget.h>
#include <imEngine/Graphics/ScreenAlignedQuad.h>

namespace imEngine {


/** @brief Сцена, поддерживающая отложенное освещение
 */
class SceneDeferred : public Scene {
public:
        /// Конструктор
        SceneDeferred(GraphicApplication* application);
        /// Деструктор
        ~SceneDeferred();

        /// Возвращает gbuffer
        RenderTarget*   gbuffer();

        /// Рендерит сцену
        void            render();
        /// Возникает при изменении размеров окна
        void            windowResizeEvent(int w, int h);

public:
        void            initGBuffer();
        void            initLBuffer();

private:
        RenderTarget            m_gbuffer;
        RenderTarget            m_lbuffer;
        ScreenAlignedQuad       m_quad;

};


} //namespace imEngine

#endif // SCENEDEFERRED_H
