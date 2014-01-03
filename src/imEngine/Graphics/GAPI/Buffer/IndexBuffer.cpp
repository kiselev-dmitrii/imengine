#include "IndexBuffer.h"
#include <imEngine/Utils/Debug.h>

namespace imEngine {


GLuint IndexBuffer::s_boundHandle = 0;

IndexBuffer::IndexBuffer() : BufferObject(BufferTarget::ELEMENT_ARRAY) {
}

void IndexBuffer::bind() {
        if (m_handle != s_boundHandle) {
                IM_GLCALL(glBindBuffer(m_target, m_handle));
                s_boundHandle = m_handle;
        }
}

void IndexBuffer::unbind() {
        IM_GLCALL(glBindBuffer(m_target, 0));
        s_boundHandle = 0;
}


} //namespace imEngine
