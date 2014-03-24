#include "PostEffectManager.h"
#include "../Scene.h"

namespace imEngine {

PostEffectManager::PostEffectManager(Scene *scene) :
        m_scene(scene)
{ }

void PostEffectManager::apply(Texture2D *lightAccum, Texture2D *diffuseBuffer,
                              Texture2D *materialBuffer, Texture2D *geometryBuffer, Texture2D *depthBuffer) {
        m_dof.setInputTexture(lightAccum);
        m_dof.setDepthTexture(depthBuffer);
        Texture2D* result = m_dof.apply();

        m_bloom.setTexture(result);
        result = m_bloom.apply();

        m_copy.setTexture(result);
        m_copy.apply();
}

Bloom* PostEffectManager::bloom() {
        return &m_bloom;
}

DepthOfField* PostEffectManager::depthOfField() {
        return &m_dof;
}

void PostEffectManager::windowResizeEvent(int w, int h) {
        IM_TODO;
}


} //namespace imEngine
