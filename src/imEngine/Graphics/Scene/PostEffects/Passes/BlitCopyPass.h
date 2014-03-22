#ifndef BLITCOPYPASS_H
#define BLITCOPYPASS_H

#include "Pass.h"

namespace imEngine {

class BlitCopyPass : public Pass {
public:
        BlitCopyPass(Texture2D* texture);
        void    setTexture(Texture2D* texture)                                  { m_texture = texture; }

protected:
        void    prepare() const;

private:
        Texture2D*      m_texture;
};


} //namespace imEngine

#endif // BLITCOPYPASS_H
