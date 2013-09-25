#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include "../../Utils/Types.h"

namespace imCore {

class Program;

/** @brief Тип шейдера.
 */
namespace ShaderType {
enum Enum {
        VERTEX                          = GL_VERTEX_SHADER,
        FRAGMENT                        = GL_FRAGMENT_SHADER,
        GEOMETRY                        = GL_GEOMETRY_SHADER,
        TESS_CONTROL                    = GL_TESS_CONTROL_SHADER,
        TESS_EVAL                       = GL_TESS_EVALUATION_SHADER
};
}


/** @brief Шейдер - часть программы, выполняемой на GPU.
 *  Подменяет определенную часть графического конвейера (подменяемая часть зависит от типа).
 *  Поскольку данный объект не будет использоваться непосредственно пользователем, то
 *  вместо конструктора и деструктора используются методы create и destroy, которые
 *  действительно создают OpenGL объекты.
 */
class Shader {
public:
        /// Конструктор.
        Shader();

        /// Создает шейдерный OpenGL объект
        void    create(ShaderType::Enum type);
        /// Удаляет шейдерную программу
        void    destroy();

        /// Загружает исходники в видеопамять из str, предварительно совершив препроцессинг
        void    uploadSource(const String& str, const StringList& defines, const String& path = "");
        /// Компилирование шейдера. Возвращает true при успехе
        bool    compile();
        /// Возвращает лог компиляци программы
        String  log();

        /// Присоединяет шейдер к программе
        void    attach(Program* program);
        /// Отсоединяет шейдер
        void    detach();

private:
        /// Осуществляет препроцесс
        String  preprocess(const String& source, const StringList& defines, const String& path);
        /// Добавляет в начало шейдера source дейфаны defines
        String  addDefines(const String& source, const StringList& defines);
        /// Обрабатывает директивы #include
        String  resolveIncludes(const String& source, const String& sourceDir);
        /// Возвращает путь из директивы #include
        String  extractPath(const String& includeString);

        /// Возвращает лог компиляции для шейдера c дескриптором handle
        String  getCompileLog(GLuint handle);
        /// Возвращает статус компиляции
        bool    getCompileStatus(GLuint handle);

private:
        GLuint                  m_handle;
        ShaderType::Enum        m_type;
        String                  m_log;
        Program*                m_program;

};

} //namespace imCore

#endif // SHADER_H
