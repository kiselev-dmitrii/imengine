#include "Renderer.h"
#include <imEngine/Utils/Debug.h>

namespace imEngine {


void Renderer::clearBuffers(GLbitfield flags) {
        IM_GLCALL(glClear(flags));
}

void Renderer::renderVertices(Primitive::Enum type, GLsizei countVertices) {
        IM_GLCALL(glDrawArrays(type, 0, countVertices));
}

void Renderer::renderIndices(Primitive::Enum type, GLsizei countIndices) {
        IM_GLCALL(glDrawElements(type, countIndices, GL_UNSIGNED_INT, (GLvoid*) 0));
}

void Renderer::beginStencilRendering() {
        IM_GLCALL(glEnable(GL_STENCIL_TEST));
        // Отключаем рендеринг во фреймбуфер и буфер глубины
        IM_GLCALL(glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE));
        IM_GLCALL(glDepthMask(GL_FALSE));

        // Включаем стенсил буфер на запись
        IM_GLCALL(glStencilFunc(GL_NEVER, 1, 0xFF));
        IM_GLCALL(glStencilOp(GL_REPLACE, GL_KEEP, GL_KEEP));
        IM_GLCALL(glStencilMask(0xFF));

        // Очищаем в стенсил буфер
        IM_GLCALL(glClear(GL_STENCIL_BUFFER_BIT));

        // Все последующие вызовы будут писать в буфер трафарета
}

void Renderer::continueStencilRendering() {
        // Включаем буфер глубины и цветовой буфер
        IM_GLCALL(glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE));
        IM_GLCALL(glDepthMask(GL_TRUE));

        // Отключаем стенсил буфер на запись и используем его
        IM_GLCALL(glStencilMask(0x00));
        IM_GLCALL(glStencilFunc(GL_EQUAL, 1, 0xFF));
}

void Renderer::endStencilRendering() {
        IM_GLCALL(glDisable(GL_STENCIL_TEST));
}

} //namespace imEngine
