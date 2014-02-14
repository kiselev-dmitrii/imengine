#ifndef BLUR_H
#define BLUR_H

#include "PostEffect.h"

namespace imEngine {

/** @brief Постэффект размытия изображения по горизонтали
 */
class HBlur : public PostEffectImplementation<HBlur> {
public:
        /// Конструктор
        HBlur();

        void    setTexture(Texture2D* texture);
        void    setSigma(float sigma);

        /// Устанавливает/убирает постэффект
        void    bind();
        void    unbind();

private:
        Texture2D*      m_texture;
        float           m_sigma;
};

/** @brief Постэффект размытия изображенрия по вертикали
 */
class VBlur : public PostEffectImplementation<VBlur> {
public:
        /// Конструктор
        VBlur();

        void    setTexture(Texture2D* texture);
        void    setSigma(float sigma);

        /// Устанавливает/убирает постэффект
        void    bind();
        void    unbind();

private:
        Texture2D*      m_texture;
        float           m_sigma;
};


/** @brief Постэффект размытия изображения
 */
class Blur : public PostEffect {
public:
        Blur();
};


} //namespace imEngine

#endif // BLUR_H
