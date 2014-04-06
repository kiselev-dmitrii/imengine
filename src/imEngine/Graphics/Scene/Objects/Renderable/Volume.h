#ifndef VOLUME_H
#define VOLUME_H

#include "../Movable.h"
#include <imEngine/Graphics/Scene/Materials/Material.h>

namespace imEngine {

/** @brief Объект для отображения объема
 *
 * Метод отображения задается выбранным материалом
 */
class Volume : public Movable {
public:
        /// Конструктор.
        Volume(Texture3D* data, const VolumeMaterialPtr& material, Object* parent);
        /// Деструктор
        ~Volume();

        /// Устанавливает/возвращает текстуру с данными
        void                     setData(Texture3D* data)                       { m_material->setData(data); }
        Texture3D*               data() const                                   { return m_material->data(); }
        /// Устанавливает/Возвращает материал
        void                     setMaterial(const VolumeMaterialPtr& material);
        const VolumeMaterialPtr& material() const;

        /// Рендерит объект с заданным материалом
        void                     render();

private:
        static GeometryPtr      s_cube;
        VolumeMaterialPtr       m_material;
};

} //namespace imEngine

#endif // VOLUME_H
