#include "Shader.h"
#include "../../Utils/Debug.h"
#include "../../Utils/GLUtils.h"
#include "../../Utils/StringUtils.h"
#include "../../System/Filesystem.h"
#include <fstream>

namespace imCore {

Shader::Shader(ShaderType::Enum type) :
        m_type(type),
        m_isCompiled(false)
{
        IM_GLCALL(m_handle = glCreateShader(type));
        IM_LOG("Shader" << m_handle << ": created, type: " << GLUtils::convertEnumToString(m_type));
}

Shader::~Shader() {
        IM_GLCALL(glDeleteShader(m_handle));
        IM_LOG("Shader" << m_handle << ": removed, type: " << GLUtils::convertEnumToString(m_type));
}

ShaderType::Enum Shader::type() {
        return m_type;
}

GLuint Shader::handle() {
        return m_handle;
}

void Shader::setSource(const String &source, const String &path) {
        m_source = source;
        m_path = path;
        m_isCompiled = false;
}

String Shader::source() {
        return m_source;
}

void Shader::loadFromFile(const String &path) {
        m_source = Filesystem::readTextFile(path);
        if (m_source == "") {
                IM_ERROR("Can't open shader " << path);
                return;
        }
        m_path = path;
        m_isCompiled = false;
}

void Shader::setMacroDefines(const StringList &defines) {
        m_macroDefines = defines;
}

StringList Shader::macroDefines() {
        return m_macroDefines;
}

bool Shader::compile() {
        IM_LOG("Shader" << m_handle << ": compiling");

        m_log.clear();
        uploadSourceToGL();
        IM_GLCALL(glCompileShader(m_handle));

        if (!compileStatus()) {
                IM_ERROR("Shader" << m_handle << ": compilation error:" << std::endl << log());
                return false;
        }

        IM_LOG("Shader" << m_handle << ": successfull compilation");
        m_isCompiled = true;
        return true;
}

String Shader::log() {
        if (!m_log.empty()) return m_log;

        // Получаем длину лога
        GLint length;
        IM_GLCALL(glGetShaderiv(m_handle, GL_INFO_LOG_LENGTH, &length));

        m_log.resize(length);
        IM_GLCALL(glGetShaderInfoLog(m_handle, length, 0, &m_log[0]));

        return m_log;
}

bool Shader::isCompiled() {
        return m_isCompiled;
}

bool Shader::compileStatus() {
        int status = GL_FALSE;
        IM_GLCALL(glGetShaderiv(m_handle, GL_COMPILE_STATUS, &status));
        return status;
}

void Shader::uploadSourceToGL() {
        String preprocessedSource = preprocessSource(m_source);

        const char* sourcePointer = preprocessedSource.c_str();
        IM_GLCALL(glShaderSource(m_handle, 1, &sourcePointer, NULL));
}

String Shader::preprocessSource(const String &source) {
        String result = addDefinesToSource(source, m_macroDefines);
        result.insert(0, "#version 330\n");
        // Если путь к GLSL файлу не был указан, то обработка инклюдов не исполняется
        if (m_path != "") result = resolveIncludes(result, Filesystem::parentPath(m_path));

        return result;
}

String Shader::addDefinesToSource(const String &source, const StringList &defineList) {
        String defines;
        for(const String& def: defineList) defines += "#define " + def + "\n";

        String result = defines + source;
        return result;
}

String Shader::resolveIncludes(const String &source, const String &sourcePathDir) {
        StringList lines = StringUtils::split(source, "\n");

        for (String &line: lines) {
                if (line.find("#include") != String::npos) {
                        auto includePath = Filesystem::joinPath(sourcePathDir, extractPathFromInclude(line));
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

String Shader::extractPathFromInclude(const String &includeString) {
        int start = includeString.find("\"") + 1;
        int end = includeString.find("\"", start) - 1;

        return includeString.substr(start, end - start + 1);
}


} //namespace imCore
