#ifndef SCREENALIGNEDQUAD_H
#define SCREENALIGNEDQUAD_H

#include "GAPI/GAPI.h"

namespace imEngine {

/** @brief Квад, развернутый на весь экран
 *
 *  Квад передает в вершинный шейдер лишь позиции [-1;1]x[-1;1]x[-1;1]
 */
class ScreenAlignedQuad {
public:
        /// Инициализирует буфера
        ScreenAlignedQuad();

        /// Рендерит квад
        void    render();

private:
        void    initBuffers();
        void    loadData();

private:
        VertexBufferPtr  m_vbo;
        VertexArrayPtr   m_vao;
};

} //namespace imEngine

#endif // SCREENALIGNEDQUAD_H
