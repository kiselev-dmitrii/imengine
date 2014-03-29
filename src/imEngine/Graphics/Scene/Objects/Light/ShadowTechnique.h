#ifndef SHADOWTECHNIQUE_H
#define SHADOWTECHNIQUE_H

#include <imEngine/Graphics/RenderTarget.h>
#include <list>
#include "../Camera/Frustum.h"
#include "../../PostEffects/Pass.h"

namespace imEngine {

class Light;
class Polygonal;
typedef std::list<Polygonal*>   PolygonalList;

/** @brief Базовый класс техники затенения
 *
 * Класс генерирования теневой карты.
 * Наследуемые классы будут реализовывать ту или иную технику
 */
class ShadowTechnique {
public:
        /// Конструктор.
        /// filename - путь к шейдеру, обеспечивающему генерирование теневой карты
        /// defines - набор дефайнов, которые будут использованы в шейдерах света, для переключения техник
        ShadowTechnique(const String& filename, const StringList& defines);

        /// Устанавливает/возвращает размер теневой карты
        void                    setShadowMapSize(const IVec2& size);
        const IVec2&            shadowMapSize() const;
        /// Возвращает усеченную пирамиду, используемую для рендера
        Frustum*                frustum();

        /// Устанавливает радиус размытости тени
        virtual void            setSmoothLevel(int radius)                      { }

        /// Возвращает список дефайнов, ассоциированных с данной техникой
        /// Данный список используется в классе Light. Когда мы меняем технику освещения
        /// требуется перекомпилировать шейдеры освещения с данными дефайнами
        const StringList&       defines() const;

        /// Рендерит теневую карту c позиции источника света light.
        /// По завершению работы, теневая карта должна быть присвоена m_shadowMap
        virtual void            updateShadowMap(Light* light, PolygonalList& objects) = 0;
        /// Возвращает теневую карту
        Texture2D*              shadowMap();

protected:
        /// Рендерит объекты objects с позиции источника света light
        void           renderObjects(Light* light, PolygonalList& objects);
        /// Обновляет размер рендер таргетов
        virtual void   updateSize(const IVec2& size) = 0;

protected:
        Program*        m_program;
        IVec2           m_size;
        Frustum         m_frustum;
        StringList      m_defines;

        Texture2D*      m_shadowMap;
};

/** @brief Умный указатель для ShadowTechnique
 */
typedef std::shared_ptr<ShadowTechnique> ShadowTechniquePtr;


/** @brief Класс, реализующий простую технику ShadowMap
 */
class SimpleShadowMapping : public ShadowTechnique {
public:
        /// Конструктор
        SimpleShadowMapping();

        /// Обновляет теневую карту
        void    updateShadowMap(Light *light, PolygonalList &objects);

protected:
        /// Обновляет размеры рендер таргетов
        void    updateSize(const IVec2 &size);

private:
        RenderTarget    m_rt;
};


/** @brief Класс, реализующий технику VSM
 */
class VarianceShadowMapping : public ShadowTechnique {
public:
        /// Конструктор
        VarianceShadowMapping();

        /// Устанавливает уровень размытости тени
        void    setSmoothLevel(int radius);

        /// Обновляет теневую карту
        void    updateShadowMap(Light *light, PolygonalList &objects);

protected:
        /// Обновляет размеры рендер таргетов
        void    updateSize(const IVec2 &size);

private:
        RenderTarget            m_rt1;
        RenderTarget            m_rt2;
        DirectionalBlurPass     m_blur;
};


} //namespace imEngine

#endif // SHADOWTECHNIQUE_H
