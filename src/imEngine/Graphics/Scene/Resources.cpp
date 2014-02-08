#include "Resources.h"
#include <imEngine/System/Filesystem.h>
#include <imEngine/Utils/Debug.h>

namespace imEngine {

//############################## Resources ###################################//

Resources::Resources() {
        m_textureMgr = new TextureManager("resources/models/textures");
        m_geometryMgr = new GeometryManager("resources/models/geometry");
}

Resources::~Resources() {
        delete m_textureMgr;
        delete m_geometryMgr;
}

Resources* Resources::instance() {
        static Resources instance;
        return &instance;
}

TextureManager* Resources::textures() {
        return m_textureMgr;
}

GeometryManager* Resources::geometry() {
        return m_geometryMgr;
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

//########################### GeometryManager ################################//

GeometryManager::GeometryManager(const String &directory) :
        ResourceManagerImplementation(directory)
{ }

Geometry* GeometryManager::geometry(const String &name) {
        Geometry* geometry = findResource(name) ;
        if (geometry) {
                return geometry;
        } else {
                Mesh mesh(Filesystem::joinPath(directory(), name));
                geometry = new Geometry(mesh);
                addResource(name, geometry);
                return geometry;
        }
}

void GeometryManager::reloadAll() {
        for (auto& elem: m_resources) {
                Geometry* geometry = elem.second;
                Mesh mesh(Filesystem::joinPath(directory(), elem.first));
                geometry->load(mesh);
        }
}


} //namespace imEngine
