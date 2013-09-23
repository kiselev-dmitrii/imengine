#ifndef PROGRAM_H
#define PROGRAM_H

#include "../../Utils/Types.h"
#include "../../Math/Common.h"
#include <map>
#include "Shader.h"

namespace imCore {

typedef std::map<String, GLuint> MapStringUint;

/** @brief Стандартные индексы вершинных входных атрибутов
 *
 *  Предполагается использовать эти индексы для связи вершинных буферов
 *  с входными атрибутами вершинных шейдеров. Также можно использовать
 *  метод attributeLocation, однако часто требуется установить связь тогда,
 *  когда еще не создано ни одного шейдера.
 *
 */
namespace ProgramAttributeLocations {
enum Enum {
        POSITION                = 0,            ///< im_vPosition
        NORMAL                  = 1,            ///< im_vNormal
        TANGENT                 = 2,            ///< im_vTangent
        TEXCOORD                = 3             ///< im_vTexCoord
};
}

/** @brief Представляет собой программу для GPU.
 *
 *  Фактически, является лишь контейнером шейдеров.
 *  Целую программу (набор шейдеров) можно полностью держать в одном файле с расширением *.glsl.
 *  После сборки программы, определяется список активных uniform переменных и их locations;
 */
class Program {
public:
        /// Создает пустую программу.
        Program();
        /// Деструктор. Разрушает программу
        ~Program();

        /// Добавляет шейдер из строки
        void            addShader(const String& source, ShaderType::Enum type, const String& path = "");
         /// Добавляет шейдер из файла
        void            addShaderFromFile(const String& path, ShaderType::Enum type);
        /// Аналогично, добавляет несколько шейдеров  (парсит текст и разбирает его)
        void            addShaders(const String& source, const String& path = "");
        /// Добавляет сразу несколько шейдеров из файла. То есть он парсит файл и разбирает его на шейдеры
        void            addShadersFromFile(const String& path);
        /// Удаляет шейдер определенного типа
        void            removeShader(ShaderType::Enum type);
        /// Удаляет все шейдеры из программы.
        void            removeAllShaders();
        /// Возвращает указатель на шейдер по типу. Если шейдер не найден, возвращает null
        Shader*         shader(ShaderType::Enum type);

       /// Устанавливает макропеременные с которыми будет скомпилирован код
        void            setMacroDefines(const StringList& defines);
        /// Возвращает набор макроопределений
        StringList      macroDefines();

        /// Собирает добавленные шейдеры в единую программу.
        /// Данную процедуру можно выполнять несколько раз, например, если исходники изменились, то можно заного пересобрать
        bool            build();
        /// Определяет, готова ли программа для использования
        bool            isBuilded();
        /// Возвращает лог сборки
        String          log();

        /// Биндит текущую программу
        void            bind();
        /// Отвязывает текущую программу от контекста
        void            unbind();

        /// Возвращает GL дескриптор программы
        GLuint          handle();

        /// Установка uniform-переменной
        bool            setUniform(const String& name, float value);
        bool            setUniform(const String& name, int value);
        bool            setUniform(const String& name, const Vec2& value);
        bool            setUniform(const String& name, const Vec3& value);
        bool            setUniform(const String& name, const Vec4& value);
        bool            setUniform(const String& name, const Mat3& value);
        bool            setUniform(const String& name, const Mat4& value);

        /// Возвращает индекс uniform переменной
        GLuint          uniformLocation(const String& name);
        /// Вовзращает индекс вершинного входного атрибута
        GLuint          attributeLocation(const String& name);

private:
        /// Компилирует шейдеры из m_shaders
        bool            compileShaders();
        /// Присеодинеяет шейдеры из m_shaders к программе
        void            attachShaders();
        /// Устанавливает стандартным входным атрибутам (im_vPosition, im_vNormal, im_vTangent, im_vTexCoord)
        /// стандартные индексы из ProgramAttributeLocations
        void            bindDefaultAttributeLocations();
        /// Линкует программу
        bool            linkProgram();
        /// Возвращает статус линковки
        bool            linkStatus();
        /// Возвращает лог линковки
        String          linkLog();
        /// Получает информацию о всех uniform-переменных в шейдерах
        void            loadUniformInformation();
        /// Получает информацию о всех аттрибутах шейдеров
        void            loadAttributeInformation();

private:
        GLuint                  m_handle;
        StringList              m_macroDefines;
        std::vector<Shader*>    m_shaders;
        String                  m_log;
        bool                    m_isBuilded;

        static GLuint           s_boundHandle;

        MapStringUint           m_uniformLocations;
        MapStringUint           m_attributeLocations;

};

} //namespace imCore

#endif // PROGRAM_H
