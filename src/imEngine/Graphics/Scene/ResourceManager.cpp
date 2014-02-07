#include "ResourceManager.h"
#include <imEngine/System/Filesystem.h>

namespace imEngine {

//############################## FileResourceManagerAbstract ###################################//

ResourceManagerAbstract::ResourceManagerAbstract(const String &directory) :
        m_directory(directory)
{ }

const String& ResourceManagerAbstract::directory() const  {
        return m_directory;
}

StringList ResourceManagerAbstract::list() const {
        StringList list = Filesystem::tree(m_directory);
        for (String& file: list) file.replace(0, m_directory.length(), "");
        return list;
}

//############################## TextureManager ###################################//

TextureManager::TextureManager(const String &directory) : ResourceManagerImplementation(directory)
{ }

Texture1D* TextureManager::texture1D(const String &name) {
        Texture1D* texture = (Texture1D*) findResource(name);
        if (texture) {
                return texture;
        } else {
                texture = new Texture1D();
                texture->load(Filesystem::joinPath(directory(),name));
                addResource(name, texture);
                return texture;
        }
}

Texture2D* TextureManager::texture2D(const String &name) {
        Texture2D* texture = (Texture2D*) findResource(name);
        if (texture) {
                return texture;
        } else {
                texture = new Texture2D();
                texture->load(Filesystem::joinPath(directory(),name));
                addResource(name, texture);
                return texture;
        }
}

void TextureManager::reloadAll() {
        for (auto& elem: m_resources) {
                Texture* texture = elem.second;
                texture->load(Filesystem::joinPath(directory(),elem.first));
        }
}

//############################## ProgramManager ###################################//


} //namespace imEngine
