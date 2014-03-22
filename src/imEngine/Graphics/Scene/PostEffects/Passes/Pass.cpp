#include "Pass.h"
#include "../../ResourceManager.h"
#include <imEngine/Graphics/ScreenAlignedQuad.h>

namespace imEngine {

Pass::Pass(const String &filename) {
        m_program = RESOURCES->programs()->program(filename);
}

void Pass::apply() const {
        static ScreenAlignedQuad quad;

        m_program->bind();
        prepare();
        quad.render();
        m_program->unbind();
}

Program* Pass::program() const {
        return m_program;
}

} //namespace imEngine
