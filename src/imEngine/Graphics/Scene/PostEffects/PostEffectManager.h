#ifndef POSTEFFECTMANAGER_H
#define POSTEFFECTMANAGER_H

#include "Bloom.h"

namespace imEngine {

class PostEffectManager {
public:
        PostEffectManager();

        const Bloom&            bloom();
        //const DepthOfField&     depthOfField();
};

} //namespace imEngine

#endif // POSTEFFECTMANAGER_H
