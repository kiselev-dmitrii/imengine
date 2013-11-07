#include "VertexArray.h"
#include <imEngine/Utils/Debug.h>

namespace imEngine {
namespace GAPI {

GLuint VertexArray::s_boundHandle = 0;

void VertexArray::create() {
        IM_GLCALL(glGenVertexArrays(1, &m_handle));
        IM_LOG("VertexArray" << m_handle << ": created");
}

void VertexArray::destroy() {
        IM_ASSERT(m_handle);

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

} //namespace GAPI
} //namespace imEngine
