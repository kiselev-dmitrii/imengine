#include "Pass.h"
#include "../../ResourceManager.h"

namespace imEngine {

ScreenAlignedQuad Pass::s_quad;

Pass::Pass(const String &filename) {
        m_program = RESOURCES->programs()->program(filename);
}

void Pass::apply() const {
        m_program->bind();
        prepare();
        s_quad.render();
        m_program->unbind();
}

Program* Pass::program() const {
        return m_program;
}

} //namespace imEngine
