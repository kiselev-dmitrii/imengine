#ifndef SCENERENDERER_H
#define SCENERENDERER_H

#include <imEngine/Graphics/RenderTarget.h>
#include "PostEffects/Bloom.h"
#include "PostEffects/DepthOfField.h"
#include "PostEffects/SSAO.h"
#include "PostEffects/Reflections.h"
#include "Scene.h"

namespace imEngine {

class Scene;
typedef std::list<Detail*> DetailPtrList;

/** @brief Занимается отрисовкой сцены, применением постэффектов и т.д
 */
class SceneRenderer {
public:
        /// Конструктор
        SceneRenderer(Scene* scene);

        /// Возвращает указатель на GBuffer и LightAccumulator
        RenderTarget*   gBuffer()                                               { return &m_gBuffer; }
        RenderTarget*   lightAccum()                                            { return &m_lightAccum; }

        /// Предоставляет интерфейс к настройкам постэффектов
        Bloom*          bloom()                                                 { return &m_bloom; }
        DepthOfField*   depthOfField()                                          { return &m_dof; }
        SSAO*           ssao()                                                  { return &m_ssao; }
        Reflections*    reflections()                                           { return &m_reflections; }

        /// Рендерит сцену
        void            render();
        /// Изменяет размеры рендер таргетов при изменении размеров окна
        void            windowResizeEvent(int w, int h);

private:
        /// Инициализируте рендертаргеты
        void            initGBuffer();
        void            initLightAccum();

        /// Группирует детали по трем спискам
        void            groupDetails(const EntityList& entities, DetailPtrList* outRegular,
                                                                   DetailPtrList* outUnlightnment,
                                                                   DetailPtrList* outTransparent);
        /// Рендерит объекты с видовой и проекционной матрицей
        void            renderRegularDetails(const DetailPtrList& details, const Mat4& viewMatrix, const Mat4& projectionMatrix);
        /// Выполяется вычисление освещения
        void            calculateLighting(const LightList& lights);
        /// Рендерит объекты неосвещаемые объекты
        void            renderUnlightenedDetails(const DetailPtrList& details, const Mat4& viewMatrix, const Mat4& projectionMatrix);
        /// Рендерит объемы
        void            renderVolumes(const VolumeList& volumes);
        /// Применяет SSAO
        void            applySSAO(Texture2D* lightAccum,
                                  Texture2D* geometryBuffer, Texture2D* depthBuffer);

        /// Применяет постэффекты и выводит финальное изображение
        void            applyPostEffects(Texture2D* lightAccum, Texture2D* diffuseBuffer,
                                         Texture2D* materialBuffer, Texture2D* geometryBuffer, Texture2D* depthBuffer);

private:
        /// Указатель на сцену
        Scene*          m_scene;

        /// Основные рендертаргеты
        RenderTarget    m_gBuffer;
        RenderTarget    m_lightAccum;

        /// Постэффекты
        SSAO            m_ssao;
        DepthOfField    m_dof;
        Bloom           m_bloom;
        Reflections     m_reflections;

        /// Копирующуй проход
        BlitCopyPass    m_copy;

};

} //namespace imEngine

#endif // SCENERENDERER_H
