#include "Material.h"
#include "../ResourceManager.h"

namespace imEngine {

Material::Material(const String &program, bool isTransparent) {
        m_program = RESOURCES->programs()->program(program);
        m_isTransparent = isTransparent;
}

Program* Material::program() const {
        return m_program;
}

bool Material::isTransparent() const {
        return m_isTransparent;
}

} //namespace imEngine
