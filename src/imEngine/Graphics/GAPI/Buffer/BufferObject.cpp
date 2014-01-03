#include "BufferObject.h"
#include <imEngine/Utils/Debug.h>
#include <imEngine/Utils/GLUtils.h>

namespace imEngine {


BufferObject::BufferObject(BufferTarget::Enum target) {
        m_size = 0;
        m_isMapped = false;
        m_target = target;

        IM_GLCALL(glGenBuffers(1, &m_handle));
        IM_LOG("BufferObject" << m_handle << ": created " << GLUtils::convertEnumToString(m_target));
}

BufferObject::~BufferObject() {
        if (isMapped()) unmap();

        IM_GLCALL(glDeleteBuffers(1, &m_handle));
        IM_LOG("BufferObject" << m_handle << ": destroyed");
}

void BufferObject::load(const void *data, GLsizeiptr size, BufferUsage::Enum usage) {
        IM_ASSERT(m_handle);
        IM_ASSERT(!m_isMapped);

        bind();
        IM_GLCALL(glBufferData(m_target, size, data, usage));
        m_size = size;
        m_usage = usage;

        IM_LOG("BufferObject" << m_handle << ": allocated " << m_size << " bytes for " << GLUtils::convertEnumToString(m_usage));
}

void BufferObject::replace(GLsizeiptr offset, GLintptr size, const void *newData) {
        IM_ASSERT(m_handle);
        IM_ASSERT(!m_isMapped);
        IM_ASSERT((offset + size) <= m_size);

        bind();
        IM_GLCALL(glBufferSubData(m_target, offset, size, newData));

        IM_LOG("BufferObject" << m_handle << ": replaced " << size << " bytes");
}

void* BufferObject::map(BufferAccess::Enum access) {
        IM_ASSERT(m_handle);
        IM_ASSERT(!m_isMapped);

        bind();
        void* pointer;
        IM_GLCALL(pointer = glMapBuffer(m_target, access));
        m_isMapped = true;

        IM_LOG("BufferObject" << m_handle << ": mapped to " << pointer);
        return pointer;
}

void* BufferObject::mapRange(GLsizeiptr offset, GLintptr size, BufferAccess::Enum access) {
        IM_ASSERT(m_handle);
        IM_ASSERT(!m_isMapped);
        IM_ASSERT((offset+size) <= m_size);

        bind();
        void* pointer;
        IM_GLCALL(pointer = glMapBufferRange(m_target, offset, size, access));
        m_isMapped = true;

        IM_LOG("BufferObject" << m_handle << ": range ["
               << offset << ";" << offset+size << "] mapped to " << pointer);
        return pointer;
}

bool BufferObject::unmap() {
        IM_ASSERT(m_handle);
        IM_ASSERT(m_isMapped);

        bind();
        bool result;
        IM_GLCALL(result = glUnmapBuffer(m_target));
        m_isMapped = false;

        IM_LOG("BufferObject" << m_handle << ": unmapped");
        return result;
}

bool BufferObject::isMapped() {
        return m_isMapped;
}

GLuint BufferObject::handle() {
        return m_handle;
}

BufferTarget::Enum BufferObject::target() {
        return m_target;
}

GLsizeiptr BufferObject::size() {
        return m_size;
}

BufferUsage::Enum BufferObject::usage() {
        return m_usage;
}


} //namespace imEngine
