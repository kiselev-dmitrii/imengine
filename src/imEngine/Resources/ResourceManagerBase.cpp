#include "ResourceManagerBase.h"
#include <imEngine/System/Filesystem.h>

namespace imEngine {

ResourceManagerBase::ResourceManagerBase(const String &directory) :
        m_directory(directory)
{ }

ResourceManagerBase::~ResourceManagerBase() {
        removeAll();
}

const String& ResourceManagerBase::directory() {
        return m_directory;
}

StringList ResourceManagerBase::list() {
        StringList list = Filesystem::tree(m_directory);
        for (String& file: list) file.replace(0, m_directory.length(), "");

        return list;
}

void ResourceManagerBase::reloadAll() {
        for(auto item: m_resources) item.second->reload();
}

void ResourceManagerBase::removeAll() {
        for(auto item: m_resources) delete item.second;
        m_resources.erase(m_resources.begin(), m_resources.end());
}

void ResourceManagerBase::addResource(ResourceBase *resource) {
        m_resources.insert(std::pair<String, ResourceBase*>(resource->name(), resource));
}

ResourceBase* ResourceManagerBase::findResource(const String &name) {
        auto it = m_resources.find(name);
        if (it == m_resources.end()) return nullptr;
        else return it->second;
}

void ResourceManagerBase::removeResource(const String &name) {
        auto it = m_resources.find(name);
        if (it == m_resources.end()) return;

        delete it->second;
        m_resources.erase(it);
}

//################################## ResourceBase ############################################//

ResourceBase::ResourceBase(const String &directory, const String &name, ResourceManagerBase *manager) :
        m_directory(directory),
        m_name(name),
        m_manager(manager)
{ }

ResourceBase::~ResourceBase() {
}

const String& ResourceBase::name() {
        return m_name;
}

void ResourceBase::setName(const String &name) {
        // Меняем в менеджере
        if (m_manager) {
                auto it = m_manager->m_resources.find(m_name);
                if (it != m_manager->m_resources.end()) {
                        m_manager->m_resources.erase(it);
                        m_manager->m_resources.insert(std::pair<String, ResourceBase*>(name, this));
                }
        }
        // Меняем у себя
        m_name = name;
}

const String& ResourceBase::directory() {
        return m_directory;
}

void ResourceBase::setDirectory(const String &directory) {
        m_directory = directory;
}

String ResourceBase::filename() {
        return Filesystem::joinPath(directory(), name());
}

ResourceManagerBase* ResourceBase::manager() {
        return m_manager;
}



} //namespace imEngine
