#include "Material.h"
#include "../ResourceManager.h"

namespace imEngine {

Material::Material(const String &program, MaterialType type) {
        m_program = RESOURCES->programs()->program(program);
        m_type = type;
}

Program* Material::program() const {
        return m_program;
}

MaterialType Material::type() const {
        return m_type;
}

} //namespace imEngine
