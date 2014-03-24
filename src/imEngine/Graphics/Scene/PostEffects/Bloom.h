#ifndef BLOOM_H
#define BLOOM_H

#include "Pass.h"
#include <imEngine/Graphics/RenderTarget.h>
#include "PostEffect.h"

namespace imEngine {

class Bloom : public PostEffect {
public:
        Bloom();

        void            setTexture(Texture2D* texture);
        void            setRadius(int radius);
        void            setStep(int step);
        void            setThreshold(float threshold);

        Texture2D*      apply();

private:
        Texture2D*              m_texture;

        DirectionalBlurPass     m_blur;
        ThresholdPass           m_threshhold;
        BlitCopyPass            m_eq;

        RenderTarget            m_rt1;
        RenderTarget            m_rt2;
};

} //namespace imEngine

#endif // BLOOM_H
