#include "ResourceManager.h"
#include <imEngine/System/Filesystem.h>

namespace imEngine {

//############################## ResourceManager ###################################//
ResourceManager::ResourceManager() {
        m_textureMgr = new TextureManager("resources/texture/");
        m_programMgr = new ProgramManager("resources/shaders/");
}

ResourceManager::~ResourceManager() {
        delete m_textureMgr;
        delete m_programMgr;
}

const TextureManager& ResourceManager::textures() {
        return *m_textureMgr;
}

const ProgramManager& ResourceManager::programs() {
        return *m_programMgr;
}

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

TextureManager::TextureManager(const String &directory) : ResourceManagerAbstract(directory)
{ }

TextureManager::~TextureManager() {
        removeAll();
}

Texture1D* TextureManager::texture1D(const String &name) {
        Texture1D* texture = (Texture1D*) findResource(name);
        if (texture) {
                return texture;
        } else {
                texture = new Texture1D();
                texture->load(Filesystem::joinPath(directory(),name));
                texture->setMinimizationFilter(TextureMinFilter::LINEAR);
                texture->setMagnificationFilter(TextureMagFilter::LINEAR);
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
                texture->setMinimizationFilter(TextureMinFilter::LINEAR);
                texture->setMagnificationFilter(TextureMagFilter::LINEAR);
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

void TextureManager::removeAll() {
        for (auto& elem: m_resources) delete elem.second;
        m_resources.clear();
}

Texture* TextureManager::findResource(const String &name) {
        auto it = m_resources.find(name);
        if (it == m_resources.end()) return nullptr;
        else return it->second;
}

void TextureManager::addResource(const String &name, Texture *texture) {
        m_resources.insert(std::pair<String, Texture*>(name, texture));
}

//############################## ProgramManager ###################################//

ProgramManager::ProgramManager(const String &directory) : ResourceManagerAbstract(directory)
{}

ProgramManager::~ProgramManager() {
        removeAll();
}

Program* ProgramManager::program(const String &name, const StringList &defines) {
        return nullptr;
}

void ProgramManager::reloadAll() {

}

void ProgramManager::removeAll() {

}


} //namespace imEngine
