#include "IndexBuffer.h"
#include "../../Utils/Debug.h"

namespace imCore {

GLuint IndexBuffer::s_boundHandle = 0;

void IndexBuffer::create() {
        BufferObject::create(BufferTarget::ELEMENT_ARRAY);
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

} //namespace imCore
