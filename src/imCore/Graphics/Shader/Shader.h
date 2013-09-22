#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include "../../Utils/Types.h"

namespace imCore {

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
 */
class Shader {
public:
        /// Создает шейдерный объект типа type
        Shader(ShaderType::Enum type);
        /// Удаляет шейдерный объект
        ~Shader();

        /// Возвращает тип шейдера
        ShaderType::Enum        type();
        /// Возвращает OpenGL дескриптор шейдера
        GLuint                  handle();

        /// Устанавливает исходный код, который затем будет скомпилирован командой compile.
        /// Здесь sourcePath необходим для того чтобы использовать относительные пути в #include в шейдерах.
        /// Если не указан, то #include директивы будут игнорироваться
        void                    setSource(const String& source, const String& path = "");
        /// Возвращает текущий используемый исходный код
        String                  source();
        /// Загружает исходный код из файла
        void                    loadFromFile(const String& path);

        /// Устанавливает макропеременные с которыми будет скомпилирован код
        void                    setMacroDefines(const StringList& defines);
        /// Возвращает набор макоропеременных с которыми будет скомпилирован код
        StringList              macroDefines();

        /// Компилирует текущий исходный код. Возвращает true, если компиляция прошла успешно
        bool                    compile();
        /// Возвращает лог компиляции. Перед компиляцией лог очищается, а после - заполняется
        String                  log();

private:
        /// Получает статус компиляции. Используется glGet* - функция вредит производительности
        bool                    compileStatus();
        /// Грузит в GL тексты шейдера в GPU
        void                    uploadSourceToGL();

        /// Делает препроцесс исходника: добавляет макроопределения, инклюдит файлы
        String                  preprocessSource(const String& source);
        /// Добавляет в начало шейдера версию дейфаны
        String                  addDefinesToSource(const String& source, const StringList& defineList);
        /// Обрабатывает директивы #include
        String                  resolveIncludes(const String& source, const String& sourcePathDir);
        /// Возвращает путь из директивы #include
        String                  extractPathFromInclude(const String& includeString);

private:
        GLuint                  m_handle;
        ShaderType::Enum        m_type;
        StringList              m_macroDefines;
        String                  m_source;
        String                  m_path;                 //путь к исходнику
        String                  m_log;
};

} //namespace imCore

#endif // SHADER_H
