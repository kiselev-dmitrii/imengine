#include "Renderbuffer.h"
#include <imEngine/Utils/Debug.h>
#include <imEngine/Utils/GLUtils.h>

namespace imEngine {


GLuint Renderbuffer::s_boundHandle = 0;


Renderbuffer::Renderbuffer() :
        m_wasMemoryAllocated(false),
        m_width(0),
        m_height(0)
{
        IM_GLCALL(glGenRenderbuffers(1, &m_handle));
        IM_LOG("Renderbuffer" << m_handle << " is created");
}

Renderbuffer::~Renderbuffer() {
        IM_GLCALL(glDeleteFramebuffers(1, &m_handle));
        IM_LOG("Renderbuffer" << m_handle << " is destroyed");
}

void Renderbuffer::allocate(int width, int height, InternalFormat::Enum internal) {
        bind();
        IM_GLCALL(glRenderbufferStorage(GL_RENDERBUFFER, internal, width, height));

        m_wasMemoryAllocated = true;
        m_internalFormat = internal;
        m_width = width;
        m_height = height;
}

bool Renderbuffer::wasMemoryAllocated() {
        return m_wasMemoryAllocated;
}

int Renderbuffer::width() {
        return m_width;
}

int Renderbuffer::height() {
        return m_height;
}

void Renderbuffer::bind() {
        if (m_handle != s_boundHandle)    {
                IM_GLCALL(glBindRenderbuffer(GL_RENDERBUFFER, m_handle));
                s_boundHandle = m_handle;
        }
}

void Renderbuffer::unbind() {
        IM_GLCALL(glBindRenderbuffer(GL_RENDERBUFFER, 0));
        s_boundHandle = 0;
}

InternalFormat::Enum Renderbuffer::internalFormat() {
        IM_ASSERT(m_wasMemoryAllocated);
        return m_internalFormat;
}

GLuint Renderbuffer::handle() {
        return m_handle;
}

} //namespace imEngine
