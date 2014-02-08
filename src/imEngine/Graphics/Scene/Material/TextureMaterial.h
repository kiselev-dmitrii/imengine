#ifndef TEXTUREMATERIAL_H
#define TEXTUREMATERIAL_H

#include "Material.h"

namespace imEngine {

/** @brief Материал с простой текстурой
 */
class TextureMaterial : public MaterialImplementation<TextureMaterial> {
public:
        /// Конструктор.
        TextureMaterial(Texture2D* texture);

        /// Устанавливает/возвращает текстуру
        void            setTexture(Texture2D* texture);
        Texture2D*      texture() const;

        /// Перегруженные методы
        void            bind();
        void            unbind();

private:
        Texture2D*      m_textureBase;
};

} //namespace imEngine

#endif // TEXTUREMATERIAL_H
