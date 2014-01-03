#include "Shader.h"
#include "Program.h"
#include <imEngine/Utils/GLUtils.h>
#include <imEngine/Utils/Debug.h>
#include <imEngine/Utils/StringUtils.h>
#include <imEngine/System/Filesystem.h>

namespace imEngine {


Shader::Shader(ShaderType::Enum type) {
        m_handle = 0;
        m_program = nullptr;
        m_type = type;

        IM_GLCALL(m_handle = glCreateShader(m_type));
        IM_LOG("Shader" << m_handle << ": created, type: " << GLUtils::convertEnumToString(m_type));
}

Shader::~Shader() {
        IM_GLCALL(glDeleteShader(m_handle));
        IM_LOG("Shader" << m_handle << ": destroyed, type: " << GLUtils::convertEnumToString(m_type));
}

void Shader::loadSource(const String &str, const StringList &defines, const String &path) {
        IM_ASSERT(m_handle);

        String source = preprocess(str, defines, path);
        const char* pointer = source.c_str();
        IM_GLCALL(glShaderSource(m_handle, 1, &pointer, NULL));
        IM_LOG("Shader" << m_handle << ": sources was loaded");
}

String Shader::preprocess(const String &source, const StringList &defines, const String &path) {
        String result = addDefines(source, defines);
        result.insert(0, "#version 330\n");
        if (path != "") result = resolveIncludes(result, Filesystem::parentPath(path));

        return result;
}

String Shader::addDefines(const String &source, const StringList &defines) {
        String defineLines;
        for(const String& def: defines) defineLines += "#define " + def + "\n";

        String result = defineLines + source;
        return result;
}

String Shader::resolveIncludes(const String &source, const String &sourceDir) {
         StringList lines = StringUtils::split(source, "\n");

        for (String &line: lines) {
                if (line.find("#include") != String::npos) {
                        auto includePath = Filesystem::joinPath(sourceDir, extractPath(line));
                        auto includeSource = Filesystem::readTextFile(includePath);
                        if (includeSource == "") {
                                IM_ERROR("Shader" << m_handle << ": compilation error: can't include file " << includePath);
                                return "";
                        }

                        line = resolveIncludes(includeSource, Filesystem::parentPath(includePath));
                }
        }
        return StringUtils::join(lines, "\n");
}

String Shader::extractPath(const String &includeString) {
        int start = includeString.find("\"") + 1;
        int end = includeString.find("\"", start) - 1;
        return includeString.substr(start, end - start + 1);
}

bool Shader::compile() {
        IM_ASSERT(m_handle);
        IM_LOG("Shader" << m_handle << ": compiling");

        m_log.clear();
        IM_GLCALL(glCompileShader(m_handle));
        m_log = getCompileLog(m_handle);

        if (!getCompileStatus(m_handle)) {
                IM_ERROR("Shader" << m_handle << ": compilation failed:" << std::endl << log());
                return false;
        } else {
                IM_LOG("Shader" << m_handle << ": successfull compilation");
                return true;
        }
}

String Shader::getCompileLog(GLuint handle) {
        String result;

        GLint length;
        IM_GLCALL(glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &length));
        result.resize(length);
        IM_GLCALL(glGetShaderInfoLog(handle, length, 0, &result[0]));

        return result;
}

bool Shader::getCompileStatus(GLuint handle) {
        int result = GL_FALSE;
        IM_GLCALL(glGetShaderiv(handle, GL_COMPILE_STATUS, &result));
        return result;
}

String Shader::log() {
        return m_log;
}

void Shader::attach(Program *program) {
        IM_ASSERT(m_handle);
        IM_ASSERT(program);

        if (!m_program) {
                m_program = program;
                IM_GLCALL(glAttachShader(m_program->handle(), m_handle));
                IM_LOG("Shader" << m_handle << ": was attached to Program" << m_program->handle());
        }
}

void Shader::detach() {
        IM_ASSERT(m_handle);

        if (m_program) {
                IM_GLCALL(glDetachShader(m_program->handle(), m_handle));
                IM_LOG("Shader" << m_handle << ": was detached from Program" << m_program->handle());
                m_program = nullptr;
        }
}


} //namespace imEngine
