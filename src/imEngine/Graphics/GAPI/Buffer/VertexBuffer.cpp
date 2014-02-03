#include "VertexBuffer.h"
#include <imEngine/Utils/Debug.h>

namespace imEngine {


VertexBuffer::VertexBuffer() : BufferObject(BufferTarget::ARRAY) { }

void VertexBuffer::connect(GLuint attributeIndex, GLuint attributeSize, GLenum attributeType, GLsizei offset, GLsizei stride) {
        bind();
        IM_GLCALL(glEnableVertexAttribArray(attributeIndex));
        IM_GLCALL(glVertexAttribPointer(attributeIndex, attributeSize, attributeType, GL_FALSE, stride, reinterpret_cast<GLubyte*>(offset)));
}


} //namespace imEngine
