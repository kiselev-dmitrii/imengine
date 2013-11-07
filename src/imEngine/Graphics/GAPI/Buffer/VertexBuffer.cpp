#include "VertexBuffer.h"
#include <imEngine/Utils/Debug.h>

namespace imEngine {
namespace GAPI {

GLuint VertexBuffer::s_boundHandle = 0;

void VertexBuffer::create() {
        BufferObject::create(BufferTarget::ARRAY);
}

void VertexBuffer::bind() {
        if (m_handle != s_boundHandle) {
                IM_GLCALL(glBindBuffer(m_target, m_handle));
                s_boundHandle = m_handle;
        }
}

void VertexBuffer::unbind() {
        IM_GLCALL(glBindBuffer(m_target, 0));
        s_boundHandle = 0;
}

void VertexBuffer::connect(GLuint attributeIndex, GLuint attributeSize, GLenum attributeType, GLsizei offset, GLsizei stride) {
        bind();
        IM_GLCALL(glEnableVertexAttribArray(attributeIndex));
        IM_GLCALL(glVertexAttribPointer(attributeIndex, attributeSize, attributeType, GL_FALSE, stride, (GLubyte *)offset));
}


} //namespace GAPI
} //namespace imEngine
