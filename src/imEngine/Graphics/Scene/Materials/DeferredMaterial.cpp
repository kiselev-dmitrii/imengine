#include "DeferredMaterial.h"
#include "../ResourceManager.h"
#include <imEngine/System/Filesystem.h>

namespace imEngine {

TexturedDeferredMaterial::TexturedDeferredMaterial() :
        Material("materials/TexturedDeferredMaterial.glsl", false)
{
        m_diffuseTexture = m_normalTexture = m_specularTexture = RESOURCES->textures()->texture2D("model/empty.png");
}

void TexturedDeferredMaterial::setDiffuseTexture(const String &name) {
        m_diffuseTexture = RESOURCES->textures()->texture2D(Filesystem::joinPath("model", name));
}

void TexturedDeferredMaterial::setNormalTexture(const String &name) {
        m_normalTexture = RESOURCES->textures()->texture2D(Filesystem::joinPath("model", name));
}

void TexturedDeferredMaterial::setSpecularTexture(const String &name) {
        m_specularTexture = RESOURCES->textures()->texture2D(Filesystem::joinPath("model", name));
}



} //namespace imEngine
