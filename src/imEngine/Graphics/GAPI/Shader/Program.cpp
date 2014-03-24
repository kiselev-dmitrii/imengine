#include "Program.h"
#include <imEngine/Utils/Debug.h>
#include <imEngine/Utils/StringUtils.h>
#include <imEngine/System/Filesystem.h>

namespace imEngine {


GLuint Program::s_boundHandle = 0;

Program::Program() {
        IM_GLCALL(m_handle = glCreateProgram());
        IM_LOG("Program" << m_handle << ": created");
}

Program::~Program() {
        IM_GLCALL(glDeleteProgram(m_handle));
        IM_LOG("Program" << m_handle << ": destroyed");
}

GLuint Program::handle() {
        return m_handle;
}

void Program::loadSource(const String &source, const String &path) {
        m_sources = parseGLSL(source);
        m_path = path;
}

Program::ShaderInfoList Program::parseGLSL(const String &source) {
        ShaderInfoList result;

        // Синтаксические особенности файла
        const String shaderSeparator = "///###";
        const StringList keywords = {"VERTEX", "FRAGMENT", "GEOMETRY", "TESS_CONTROL", "TESS_EVAL"};
        const std::vector<ShaderType::Enum> shaderTypes = {ShaderType::VERTEX, ShaderType::FRAGMENT, ShaderType::GEOMETRY, ShaderType::TESS_CONTROL, ShaderType::TESS_EVAL};

        // Разбиваем весь исходный код по спец. символу
        StringList sources = StringUtils::split(source, shaderSeparator);
        for (String& source: sources) source.insert(0, shaderSeparator);

        // Определяем тип шейдера по коду и добавляем в результат
        for (String& source: sources) for (size_t i = 0; i < keywords.size(); ++i) {
                // Если в первой строчке найден тип шейдера, то добовляем его
               if (source.substr(0, 30).find(keywords[i]) != String::npos) {
                       ShaderInfo info;
                       info.source = source;
                       info.type = shaderTypes[i];
                       result.push_back(info);
               }
        }

        return result;
}

void Program::loadSourceFromFile(const String &path) {
        String source = Filesystem::readTextFile(path);
        if (source == "") {
                IM_ERROR("Program" << m_handle << ": can't read file " << path);
                return;
        }
        loadSource(source, path);
}

String Program::source(ShaderType::Enum type) {
        for (ShaderInfo& info: m_sources) {
                if (info.type == type) return info.source;
        }
        return "";
}

void Program::setDefines(const StringList &defines) {
        m_defines = defines;
}

StringList Program::defines() {
        return m_defines;
}

bool Program::build() {
        IM_LOG("Program" << m_handle << ": building");

        m_log.clear();
        ShaderList shaders = createShaders(m_sources, m_defines, m_path);
        bindDefaultAttributeLocations();
        bool linkingResult = linkProgram();
        destroyShaders(shaders);

        if (linkingResult) {
                // Получаем информацию о индексах атрибутов и юниформов
                loadUniformInformation();
                loadAttributeInformation();
        }

        return linkingResult;
}

Program::ShaderList Program::createShaders(const ShaderInfoList &sources, const StringList &defines, const String &path) {
        IM_LOG("Program" << m_handle << ": creating all shaders...");
        ShaderList result;

        for(const ShaderInfo& info: sources) {
                Shader* shader = new Shader(info.type);
                shader->loadSource(info.source, defines, path);
                shader->compile();
                shader->attach(this);
                m_log += shader->log();

                result.push_back(shader);
        }

        return result;
}

void Program::bindDefaultAttributeLocations() {
        IM_GLCALL(glBindAttribLocation(m_handle, ProgramAttributeLocations::POSITION, "im_vPosition"));
        IM_GLCALL(glBindAttribLocation(m_handle, ProgramAttributeLocations::NORMAL, "im_vNormal"));
        IM_GLCALL(glBindAttribLocation(m_handle, ProgramAttributeLocations::TANGENT, "im_vTangent"));
        IM_GLCALL(glBindAttribLocation(m_handle, ProgramAttributeLocations::TEXCOORD, "im_vTexCoord"));
}

bool Program::linkProgram() {
        IM_LOG("Program" << m_handle << ": linking...");

        IM_GLCALL(glLinkProgram(m_handle));
        m_log += "\n" + getLinkLog(m_handle);

        if (!getLinkStatus(m_handle)) {
                IM_ERROR("Program" << m_handle << ": linking error:" << std::endl << log());
                return false;
        } else {
                IM_LOG("Program" << m_handle << ": successfull linking");
                return true;
        }
}

bool Program::getLinkStatus(GLuint handle) {
        GLint result = GL_FALSE;
        IM_GLCALL(glGetProgramiv(handle, GL_LINK_STATUS, &result));
        return result;
}

String Program::getLinkLog(GLuint handle) {
        String result;

        GLint length;
        IM_GLCALL(glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &length));
        result.resize(length);
        IM_GLCALL(glGetProgramInfoLog(handle, length, 0, &result[0]));

        return result;
}

void Program::destroyShaders(ShaderList &shaders) {
        IM_LOG("Program" << m_handle << ": destroying all shaders...");

        for(Shader* shader: shaders) {
                shader->detach();
                delete shader;
        }
}

String Program::log() {
        return m_log;
}

void Program::bind() {
        if (m_handle != s_boundHandle) {
                IM_GLCALL(glUseProgram(m_handle));
                s_boundHandle = m_handle;

                //IM_LOG("Program" << m_handle << ": bound to context");
        }
}

void Program::unbind() {
        IM_GLCALL(glUseProgram(0));
        s_boundHandle = 0;
}

bool Program::setUniform(const String &name, float value) {
        bind();
        GLuint location = uniformLocation(name);
        if (location < 0 ) return false;
        IM_GLCALL(glUniform1f(location, value));
        return true;
}

bool Program::setUniform(const String &name, int value) {
        bind();
        GLuint location = uniformLocation(name);
        if (location < 0 ) return false;
        IM_GLCALL(glUniform1i(location, value));
        return true;
}

bool Program::setUniform(const String &name, const Vec2 &value) {
        bind();
        GLuint location = uniformLocation(name);
        if (location < 0 ) return false;
        IM_GLCALL(glUniform2f(location, value.x, value.y));
        return true;
}

bool Program::setUniform(const String &name, const Vec3 &value) {
        bind();
        GLuint location = uniformLocation(name);
        if (location < 0 ) return false;
        IM_GLCALL(glUniform3f(location, value.x, value.y, value.z));
        return true;
}

bool Program::setUniform(const String &name, const Vec4 &value) {
        bind();
        GLuint location = uniformLocation(name);
        if (location < 0 ) return false;
        IM_GLCALL(glUniform4f(location, value.x, value.y, value.z, value.w));
        return true;
}

bool Program::setUniform(const String &name, const Mat3 &value) {
        bind();
        GLuint location = uniformLocation(name);
        if (location < 0 ) return false;
        IM_GLCALL(glUniformMatrix3fv(location, 1, GL_FALSE, &value[0][0]));
        return true;
}

bool Program::setUniform(const String &name, const Mat4 &value) {
        bind();
        GLuint location = uniformLocation(name);
        if (location < 0 ) return false;
        IM_GLCALL(glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]));
        return true;
}

bool Program::setUniform(const String &name, const float* array, uint size) {
        bind();
        GLuint location = uniformLocation(name);
        if (location < 0 ) return false;
        IM_GLCALL(glUniform1fv(location, size, array));
        return true;
}

bool Program::setUniform(const String &name, const int* array, uint size) {
        bind();
        GLuint location = uniformLocation(name);
        if (location < 0 ) return false;
        IM_GLCALL(glUniform1iv(location, size, array));
        return true;
}

bool Program::setUniform(const String &name, const Vec2* array, uint size) {
        bind();
        GLuint location = uniformLocation(name);
        if (location < 0 ) return false;
        IM_GLCALL(glUniform2fv(location, size, &(array[0].x)));
        return true;
}

bool Program::setUniform(const String &name, const Vec3* array, uint size) {
        bind();
        GLuint location = uniformLocation(name);
        if (location < 0 ) return false;
        IM_GLCALL(glUniform3fv(location, size, &(array[0].x)));
        return true;
}

GLuint Program::uniformLocation(const String &name) {
        auto it = m_uniformLocations.find(name);
        if (it == m_uniformLocations.end()) return -1;
        return it->second;
}

GLuint Program::attributeLocation(const String &name) {
        auto it = m_attributeLocations.find(name);
        if (it == m_attributeLocations.end()) return -1;
        return it->second;
}

void Program::loadUniformInformation() {
        // Определяем количество uniform-переменных
        GLint count;
        IM_GLCALL(glGetProgramiv(m_handle, GL_ACTIVE_UNIFORMS, &count));

        // Определяем максимальную длину среди имен переменных
        GLint maxLength;
        IM_GLCALL(glGetProgramiv(m_handle, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxLength));

        GLchar* name = new GLchar[maxLength];
        for(int i = 0; i < count; ++i) {
                GLsizei written;
                GLint   size;
                GLenum  type;
                // Получение информации о переменной. Здесь i - это НЕ uniform location
                IM_GLCALL(glGetActiveUniform(m_handle, i, maxLength, &written, &size, &type, name));

                // Получение uniform location
                GLint location;
                IM_GLCALL(location = glGetUniformLocation(m_handle, name));

                // Кэшируем
                m_uniformLocations[String(name)] = location;
        }
        delete [] name;

}

void Program::loadAttributeInformation() {
        // Определяем количество атрибутов
        GLint count;
        IM_GLCALL(glGetProgramiv(m_handle, GL_ACTIVE_ATTRIBUTES, &count));
        // Определяем максимальную длину среди имен переменных
        GLint maxLength;
        IM_GLCALL(glGetProgramiv(m_handle, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxLength));

        GLchar* name = new GLchar[maxLength];
        for(int i = 0; i < count; ++i) {
                GLsizei written;
                GLint   size;
                GLenum  type;
                // Получение информации о переменной
                IM_GLCALL(glGetActiveAttrib(m_handle, i, maxLength, &written, &size, &type, name));

                // Получение attribute location
                GLint location;
                IM_GLCALL(location = glGetAttribLocation(m_handle, name));

                // Кэшируем
                m_attributeLocations[String(name)] = location;
        }
        delete [] name;
}


} //namespace imEngine
