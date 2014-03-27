#include "PostEffectManager.h"
#include "../Scene.h"

namespace imEngine {

PostEffectManager::PostEffectManager(Scene *scene) :
        m_scene(scene)
{ }

void PostEffectManager::apply(Texture2D *lightAccum, Texture2D *diffuseBuffer,
                              Texture2D *materialBuffer, Texture2D *geometryBuffer, Texture2D *depthBuffer) {
        Texture2D* result = lightAccum;

        m_ssao.setInputTexture(result);
        m_ssao.setNormalTexture(geometryBuffer);
        m_ssao.setDepthTexture(depthBuffer);
        m_ssao.setActiveCamera(m_scene->activeCamera());
        result = m_ssao.apply();

        m_dof.setInputTexture(result);
        m_dof.setDepthTexture(depthBuffer);
        result = m_dof.apply();

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

SSAO* PostEffectManager::ssao() {
        return &m_ssao;
}

void PostEffectManager::windowResizeEvent(int w, int h) {
        IM_TODO;
}


} //namespace imEngine
