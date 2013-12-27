#include "ResourceManager.h"

namespace imEngine {

ResourceManager::ResourceManager() {
        m_textureMgr = new TextureManager("resources/texture");
}

ResourceManager::~ResourceManager() {
        delete m_textureMgr;
}

TextureManager* ResourceManager::textures() {
        return m_textureMgr;
}

//##################### TextureManager ######################//

TextureManager::TextureManager(const String &directory) : ResourceManagerBase(directory)
{}

Texture1D* TextureManager::texture1D(const String &name) {
        Texture1D* result = (Texture1D*) findResource(name);
        if (!result) {
                result = new Texture1D(directory(), name, this);
                ((Texture1D*)result)->load();
                result->raw()->setMagnificationFilter(GAPI::TextureMagFilter::LINEAR);
                result->raw()->setMinimizationFilter(GAPI::TextureMinFilter::LINEAR);

                addResource(result);
        }
        return result;
}

Texture2D* TextureManager::texture2D(const String &name) {
        Texture2D* result = (Texture2D*) findResource(name);
        if (!result) {
                result = new Texture2D(directory(), name, this);
                ((Texture2D*)result)->load();
                result->raw()->setMagnificationFilter(GAPI::TextureMagFilter::LINEAR);
                result->raw()->setMinimizationFilter(GAPI::TextureMinFilter::LINEAR);

                addResource(result);
        }
        return result;
}

} //namespace imEngine
