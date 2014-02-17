#include "ResourceManager.h"
#include <imEngine/System/Filesystem.h>
#include <imEngine/Utils/Debug.h>

namespace imEngine {

//############################## Resources ###################################//

ResourceManager::ResourceManager() {
        m_programMgr = new ProgramManager("resources/shaders");
        m_textureMgr = new TextureManager("resources/models/textures");
        m_geometryMgr = new GeometryManager("resources/models/geometry");
}

ResourceManager::~ResourceManager() {
        delete m_textureMgr;
        delete m_geometryMgr;
}

ResourceManager* ResourceManager::instance() {
        static ResourceManager instance;
        return &instance;
}

TextureManager* ResourceManager::textures() {
        return m_textureMgr;
}

GeometryManager* ResourceManager::geometry() {
        return m_geometryMgr;
}

ProgramManager* ResourceManager::programs() {
        return m_programMgr;
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
                texture->load(Filesystem::joinPath(directory(), elem.first));
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


//########################### GeometryManager ################################//

ProgramManager::ProgramManager(const String &directory) :
        ResourceManagerAbstract(directory)
{ }

ProgramManager::~ProgramManager() {
        removeAll();
}

Program* ProgramManager::program(const String &name, const StringList &defines) {
        Program* program = findResource(name, defines);
        if (program) {
                return program;
        } else {
                program = new Program();
                program->loadSourceFromFile(Filesystem::joinPath(directory(), name));
                program->setDefines(defines);
                program->build();
                addResource(name, program);
                return program;
        }
}

Program* ProgramManager::program(const String &name) {
        return program(name, {});
}

void ProgramManager::reloadAll() {
        for (auto& item: m_resources) {
                Program* program = item.second;
                program->loadSourceFromFile(Filesystem::joinPath(directory(), item.first));
                program->build();
        }
}

void ProgramManager::removeAll() {
        for (auto& item: m_resources) delete item.second;
        m_resources.clear();
}

Program* ProgramManager::findResource(const String &name, const StringList &defines) {
        auto p = m_resources.equal_range(name);
        auto begin = p.first;
        auto end = p.second;

        // Проходимся по всем программам у которых имена name
        for (auto it = begin; it != end; ++it) {
                Program* program = (*it).second;
                if (program->defines() == defines) return program;
        }
        return nullptr;
}

void ProgramManager::addResource(const String &name, Program *resource) {
        m_resources.insert(std::pair<String, Program*>(name, resource));
}

} //namespace imEngine
