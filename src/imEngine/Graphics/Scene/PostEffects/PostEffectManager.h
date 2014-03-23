#ifndef POSTEFFECTMANAGER_H
#define POSTEFFECTMANAGER_H

#include "Bloom.h"
#include "DepthOfField.h"

namespace imEngine {
class Scene;

/** @brief Менеджер постэффектов.
 */
class PostEffectManager {
public:
        /// Конструктор. Получает указатель на сцену
        explicit PostEffectManager(Scene* scene);

        /// Применяет постэффекты используя для этого переданные буферы
        void                    apply(Texture2D* lightAccum, Texture2D* diffuseBuffer,
                                      Texture2D* materialBuffer, Texture2D* geometryBuffer, Texture2D* depthBuffer);
        /// Изменяет размеры рендер таргетов при изменении размеров окна
        void                    windowResizeEvent(int w, int h);

        /// Предоставляет интерфейс к настройкам постэффектов
        const Bloom&            bloom()                                         { return m_bloom; }
        const DepthOfField&     depthOfField()                                  { return m_dof; }
        //const Fog&              fog();
        //const SSAO&             ssao();

private:
        Scene*          m_scene;

        Bloom           m_bloom;
        DepthOfField    m_dof;
        BlitCopyPass    m_copy;
};

} //namespace imEngine

#endif // POSTEFFECTMANAGER_H
