#include "Program.h"
#include "../../Utils/Debug.h"
#include "../../Utils/GLUtils.h"
#include "../../Utils/StringUtils.h"
#include "../../System/Filesystem.h"

namespace imCore {

GLuint Program::s_boundHandle = 0;

Program::Program() :
        m_isBuilded(false)
{
        IM_GLCALL(m_handle = glCreateProgram());
        IM_LOG("Program" << m_handle << ": created");
}

Program::~Program() {
        removeAllShaders();

        IM_GLCALL(glDeleteProgram(m_handle));
        IM_LOG("Program" << m_handle << ": removed");
}

void Program::addShader(const String &source, ShaderType::Enum type, const String &path) {
        if (this->shader(type)) {
                IM_ERROR("Program" << m_handle << ": shader " << GLUtils::convertEnumToString(type) << " was already added");
                return;
        }

        Shader* shader = new Shader(type);
        shader->setSource(source, path);
        shader->attachToProgram(this);
        m_shaders.push_back(shader);
}

void Program::addShaderFromFile(const String &path, ShaderType::Enum type) {
        if (this->shader(type)) {
                IM_ERROR("Program" << m_handle << ": shader " << GLUtils::convertEnumToString(type) << " was already added");
                return;
        }

        Shader* shader = new Shader(type);
        shader->loadFromFile(path);
        shader->attachToProgram(this);
        m_shaders.push_back(shader);
}

void Program::addShaders(const String &source, const String &path) {
        const String shaderSeparator = "///###";
        const StringList keywords = {"VERTEX", "FRAGMENT", "GEOMETRY", "TESS_CONTROL", "TESS_EVAL"};
        const std::vector<ShaderType::Enum> shaderTypes = {ShaderType::VERTEX, ShaderType::FRAGMENT, ShaderType::GEOMETRY, ShaderType::TESS_CONTROL, ShaderType::TESS_EVAL};

        // Разбиваем весь исходный код по спец. символу
        StringList sources = StringUtils::split(source, shaderSeparator);
        for (String& source: sources) source.insert(0, shaderSeparator);

        // Определяем тип шейдера по коду и создаем соотв. шейдер
        for (String& source: sources) for (size_t i = 0; i < keywords.size(); ++i) {
                // Если в первой строчке найден тип шейдера, то добовляем его
               if (source.substr(0, 30).find(keywords[i]) != String::npos) addShader(source, shaderTypes[i], path);
        }
}

void Program::addShadersFromFile(const String &path) {
        String source = Filesystem::readTextFile(path);
        if (source == "") {
                IM_ERROR("Can't add shader from file " << path);
                return;
        }

        addShaders(source, path);
}

void Program::removeShader(ShaderType::Enum type) {
        for (auto it = m_shaders.begin(); it != m_shaders.end(); ++it) {
                if ((*it)->type() == type) {
                        (*it)->detachFromProgram();
                        delete (*it);
                        m_shaders.erase(it);
                        return;
                }
        }
        IM_ERROR("Program" << m_handle << ": can't find shader with type " << GLUtils::convertEnumToString(type));
}

void Program::removeAllShaders() {
        for (Shader* shader: m_shaders) {
                shader->detachFromProgram();
                delete shader;
        }
        m_shaders.clear();
}

Shader* Program::shader(ShaderType::Enum type) {
        for (Shader* shader: m_shaders) {
                if (shader->type() == type) return shader;
        }
        return nullptr;
}

void Program::setMacroDefines(const StringList &defines) {
        m_macroDefines = defines;
}

StringList Program::macroDefines() {
        return m_macroDefines;
}

bool Program::build() {
        // Удаляет информацию, которая генерируется в процессе сборки
        m_log.clear();
        m_uniformLocations.clear();
        m_attributeLocations.clear();

        // Производим непосредственно сборку
        if (!compileShaders()) {
                IM_ERROR("Program" << m_handle << ": can't compile shaders.");
                return false;
        }
        bindDefaultAttributeLocations();
        if (!linkProgram()) {
                IM_ERROR("Program" << m_handle << ": can't link shaders." << std::endl << log());
                return false;
        }
        m_isBuilded = true;

        // Получаем информацию о индексах атрибутов и юниформов
        loadUniformInformation();
        loadAttributeInformation();

        IM_LOG("Program" << m_handle << ": building is complete");

        return true;
}

bool Program::isBuilded() {
        return m_isBuilded;
}

String Program::log() {
        return m_log;
}

void Program::bind() {
        if (s_boundHandle != m_handle) {
                IM_GLCALL(glUseProgram(m_handle));
                s_boundHandle = m_handle;

                IM_LOG("Program" << m_handle << ": bound to context");
        }
}

void Program::unbind() {
        IM_GLCALL(glUseProgram(0));
        s_boundHandle = 0;
}

GLuint Program::handle() {
        return m_handle;
}

bool Program::setUniform(const String &name, float value) {
        GLuint location = uniformLocation(name);
        if (location < 0 ) return false;
        IM_GLCALL(glUniform1f(location, value));
        return true;
}

bool Program::setUniform(const String &name, int value) {
        GLuint location = uniformLocation(name);
        if (location < 0 ) return false;
        IM_GLCALL(glUniform1i(location, value));
        return true;
}

bool Program::setUniform(const String &name, const Vec2 &value) {
        GLuint location = uniformLocation(name);
        if (location < 0 ) return false;
        IM_GLCALL(glUniform2f(location, value.x, value.y));
        return true;
}

bool Program::setUniform(const String &name, const Vec3 &value) {
        GLuint location = uniformLocation(name);
        if (location < 0 ) return false;
        IM_GLCALL(glUniform3f(location, value.x, value.y, value.z));
        return true;
}

bool Program::setUniform(const String &name, const Vec4 &value) {
        GLuint location = uniformLocation(name);
        if (location < 0 ) return false;
        IM_GLCALL(glUniform4f(location, value.x, value.y, value.z, value.w));
        return true;
}

bool Program::setUniform(const String &name, const Mat3 &value) {
        GLuint location = uniformLocation(name);
        if (location < 0 ) return false;
        IM_GLCALL(glUniformMatrix3fv(location, 1, GL_FALSE, &value[0][0]));
        return true;
}

bool Program::setUniform(const String &name, const Mat4 &value) {
        GLuint location = uniformLocation(name);
        if (location < 0 ) return false;
        IM_GLCALL(glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]));
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

bool Program::compileShaders() {
        for (Shader* shader: m_shaders) {
                shader->setMacroDefines(m_macroDefines);

                if (!shader->compile()) {
                        m_log += shader->log();
                        return false;
                }

                m_log += shader->log();
        }
        return true;
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
        m_log += "\n" + linkLog();
        return linkStatus();
}

bool Program::linkStatus() {
        GLint result = GL_FALSE;
        IM_GLCALL(glGetProgramiv(m_handle, GL_LINK_STATUS, &result));
        return result;
}

String Program::linkLog() {
        String result;

        GLint length;
        IM_GLCALL(glGetProgramiv(m_handle, GL_INFO_LOG_LENGTH, &length));
        result.resize(length);

        IM_GLCALL(glGetProgramInfoLog(m_handle, length, 0, &result[0]));
        return result;
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

} //namespace imCore
