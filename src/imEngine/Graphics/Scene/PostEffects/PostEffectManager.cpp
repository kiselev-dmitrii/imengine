#include "PostEffectManager.h"
#include "../Scene.h"

namespace imEngine {

PostEffectManager::PostEffectManager(Scene *scene) :
        m_scene(scene)
{ }

void PostEffectManager::apply(Texture2D *lightAccum, Texture2D *diffuseBuffer,
                              Texture2D *materialBuffer, Texture2D *geometryBuffer, Texture2D *depthBuffer) {
        /*
        m_dof.setInputTexture(lightAccum);
        m_dof.setDepthTexture(depthBuffer);
        m_dof.apply();
        */
        m_bloom.setTexture(lightAccum);
        m_bloom.apply();
}

void PostEffectManager::windowResizeEvent(int w, int h) {
        IM_TODO;
}


} //namespace imEngine
