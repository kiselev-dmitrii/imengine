#include "Renderer.h"
#include <imEngine/Utils/Debug.h>

namespace imEngine {

RenderState Renderer::s_renderState = {
        BlendMode::NONE,
        DepthMode::NONE,
        CullMode::BACK,
        PolygonMode::FILL
};

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

void Renderer::setBlendMode(BlendMode::Enum blendMode) {
        if (s_renderState.blendMode == blendMode) return;

        if (blendMode != BlendMode::NONE) IM_GLCALL(glEnable(GL_BLEND));

        switch (blendMode) {
                case BlendMode::ADD:
                        IM_GLCALL(glBlendFunc(GL_ONE, GL_ONE));
                        IM_GLCALL(glBlendEquation(GL_FUNC_ADD));
                        break;

                case BlendMode::SCREEN:
                        IM_GLCALL(glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR));
                        IM_GLCALL(glBlendEquation(GL_FUNC_ADD));
                        break;

                case BlendMode::LIGHTEN:
                        IM_GLCALL(glBlendFunc(GL_ONE, GL_ONE));
                        IM_GLCALL(glBlendEquation(GL_MAX));
                        break;

                case BlendMode::MULTIPLY:
                        IM_GLCALL(glBlendFunc(GL_DST_COLOR, GL_ZERO));
                        IM_GLCALL(glBlendEquation(GL_FUNC_ADD));
                        break;

                case BlendMode::DARKEN:
                        IM_GLCALL(glBlendFunc(GL_ONE, GL_ONE));
                        IM_GLCALL(glBlendEquation(GL_MIN));
                        break;

                case BlendMode::BURN:
                        IM_GLCALL(glBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_DST_COLOR));
                        IM_GLCALL(glBlendEquation(GL_FUNC_SUBTRACT));
                        break;

                case BlendMode::ALPHA:
                        IM_GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
                        IM_GLCALL(glBlendEquation(GL_FUNC_ADD));
                        break;

                case BlendMode::NONE:
                        IM_GLCALL(glDisable(GL_BLEND)) ;
                        break;

                default:
                        break;
        }
        s_renderState.blendMode = blendMode;
}

void Renderer::setDepthMode(DepthMode::Enum depthMode) {
        if (s_renderState.depthMode == depthMode) return;

        if (depthMode != DepthMode::NONE) {
               IM_GLCALL(glEnable(GL_DEPTH_TEST));
               IM_GLCALL(glDepthFunc(depthMode));
        } else {
               IM_GLCALL(glDisable(GL_DEPTH_TEST));
        }
        s_renderState.depthMode = depthMode;
}

void Renderer::setCullMode(CullMode::Enum cullMode) {
        if (s_renderState.cullMode == cullMode) return;

        if (cullMode != CullMode::NONE) {
                IM_GLCALL(glEnable(GL_CULL_FACE));
                IM_GLCALL(glCullFace(cullMode));
        } else {
                IM_GLCALL(glDisable(GL_CULL_FACE));
        }
        s_renderState.cullMode = cullMode;
}

void Renderer::setPolygonMode(PolygonMode::Enum polygonMode) {
        if (s_renderState.polygonMode == polygonMode) return;

        IM_GLCALL(glPolygonMode(GL_FRONT_AND_BACK, polygonMode));
        s_renderState.polygonMode = polygonMode;
}

void Renderer::setRenderState(const RenderState &renderState) {
        setBlendMode(renderState.blendMode);
        setDepthMode(renderState.depthMode);
        setCullMode(renderState.cullMode);
        setPolygonMode(renderState.polygonMode);
}

const RenderState& Renderer::renderState() {
        return s_renderState;
}

} //namespace imEngine
