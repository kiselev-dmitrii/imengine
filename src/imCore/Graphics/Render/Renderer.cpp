#include "Renderer.h"
#include "../../Utils/Debug.h"

namespace imCore {

void Renderer::clearBuffers(GLbitfield flags) {
        IM_GLCALL(glClear(flags));
}

void Renderer::renderVertices(Primitive::Enum type, GLsizei countVertices) {
        IM_GLCALL(glDrawArrays(type, 0, countVertices));
}

void Renderer::renderIndices(Primitive::Enum type, GLsizei countIndices) {
        IM_GLCALL(glDrawElements(type, countIndices, GL_UNSIGNED_INT, (GLvoid*) 0));
}

} //namespace imCore