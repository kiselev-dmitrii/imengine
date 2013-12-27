#include "Texture.h"

namespace imEngine {

//################################## Texture1D ################################//

Texture1D::Texture1D(const String &directory, const String &name, ResourceManagerBase *manager) :
        ResourceBase(directory, name, manager)
{ }

Texture1D::~Texture1D() {
        m_texture.destroy();
}

void Texture1D::load() {
        m_texture.create();
        m_texture.load(filename());
}

void Texture1D::reload() {
        m_texture.destroy();
        load();
}

//################################## Texture2D ################################//

Texture2D::Texture2D(const String &directory, const String &name, ResourceManagerBase *manager) :
        ResourceBase(directory, name, manager)
{}

Texture2D::~Texture2D() {
}

void Texture2D::load() {
        m_texture.create();
        m_texture.load(filename());
}

void Texture2D::reload() {
        m_texture.destroy();
        load();
}


} //namespace imEngine
