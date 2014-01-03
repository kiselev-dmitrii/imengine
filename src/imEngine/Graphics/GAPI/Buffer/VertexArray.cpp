#include "VertexArray.h"
#include <imEngine/Utils/Debug.h>

namespace imEngine {


GLuint VertexArray::s_boundHandle = 0;

VertexArray::VertexArray() {
        IM_GLCALL(glGenVertexArrays(1, &m_handle));
        IM_LOG("VertexArray" << m_handle << ": created");
}

VertexArray::~VertexArray() {
        IM_GLCALL(glDeleteVertexArrays(1, &m_handle));
        IM_LOG("VertexArray" << m_handle << ": destroyed");
}

GLuint VertexArray::handle() {
        return m_handle;
}

void VertexArray::bind() {
        if (m_handle != s_boundHandle) {
                IM_GLCALL(glBindVertexArray(m_handle));
                s_boundHandle = m_handle;
        }
}

void VertexArray::unbind() {
        IM_GLCALL(glBindVertexArray(0)) ;
        s_boundHandle = 0;
}


} //namespace imEngine
