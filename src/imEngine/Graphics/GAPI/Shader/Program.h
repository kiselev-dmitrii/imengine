#ifndef PROGRAM_H
#define PROGRAM_H

#include <map>
#include "Shader.h"
#include <imEngine/Utils/Types.h>
#include <imEngine/Math/Common.h>

namespace imEngine {


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
 *  Целую программу (набор шейдеров) можно полностью держать в одном файле с расширением *.glsl.
 *  После сборки программы, определяется список активных uniform переменных и их locations;
 */
class Program {
public:
        /// Конструктор. Создает программу
        Program();
        /// Деструктор. Уничтожает программу
        ~Program();

        /// Возвращает OpenGL дескриптор программы
        GLuint          handle();

        /// Загружает программу из строки содержащей исходный код
        void            loadSource(const String& source, const String& path = "");
        /// Загружает программу из glsl файла.
        void            loadSourceFromFile(const String& path);
        /// Возвращает исходник шейдера определенного типа или пустую строку, если шейдера с данными типом нет
        String          source(ShaderType::Enum type);

        /// Устанавливает дефайны, с которыми будет компилироваться исходник
        void            setDefines(const StringList& defines);
        /// Возвращает список дефайнов
        StringList      defines();

        /// Осуществляет сборку программы. Возвращает true, если сборка осуществлена успешно
        bool            build();
        /// Возвращает лог сборки
        String          log();

        /// Биндит текущую программу
        void            bind();
        /// Отвязывает текущую программу от контекста
        void            unbind();

        /// Установка uniform-переменной.  Возвращают false, если переменная с данным именем отсутствует в программе.
        bool            setUniform(const String& name, float value);
        bool            setUniform(const String& name, int value);
        bool            setUniform(const String& name, const Vec2& value);
        bool            setUniform(const String& name, const Vec3& value);
        bool            setUniform(const String& name, const Vec4& value);
        bool            setUniform(const String& name, const Mat3& value);
        bool            setUniform(const String& name, const Mat4& value);

        /// Возвращает индекс uniform переменной или -1, если переменная отсутствует в программе
        GLuint          uniformLocation(const String& name);
        /// Вовзращает индекс вершинного входного атрибута или -1, если переменная отсутствует в программе
        GLuint          attributeLocation(const String& name);

private:
        /// Структура, по которой можно создать шейдер
        struct ShaderInfo {
                ShaderType::Enum        type;
                String                  source;
        };
        typedef std::vector<ShaderInfo>         ShaderInfoList;
        typedef std::vector<Shader*>            ShaderList;
        typedef std::map<String, GLuint>        MapStringUint;

private:
        /// Разбирает на состовляющие *.glsl файл
        ShaderInfoList  parseGLSL(const String& source);

        /// Создает, компилирует и присоединяет OpenGL шейдеры
        ShaderList      createShaders(const ShaderInfoList& sources, const StringList& defines, const String& path);
        /// Устанавливает стандартным атрибутам индексы из ProgramAttributeLocations
        void            bindDefaultAttributeLocations();
        /// Линкует программу
        bool            linkProgram();
        /// Возвращает OpenGL статус линковки
        bool            getLinkStatus(GLuint handle);
        /// Возвращает OpenGL лог линковки
        String          getLinkLog(GLuint handle);
        /// Отсоединяет и удаляет OpenGL шейдеры
        void            destroyShaders(ShaderList& shaders);

        /// Получает информацию о всех uniform-переменных в шейдерах
        void            loadUniformInformation();
        /// Получает информацию о всех аттрибутах шейдеров
        void            loadAttributeInformation();

private:
        /// Данные касающиеся создания шейдеров
        ShaderInfoList  m_sources;
        StringList      m_defines;
        String          m_path;

        /// Данные касающиеся самой программы
        GLuint          m_handle;
        String          m_log;

        /// Кэш индексов переменных в программе
        MapStringUint   m_uniformLocations;
        MapStringUint   m_attributeLocations;

        /// Текущая используемая программа
        static GLuint   s_boundHandle;

};


} //namespace imEngine

#endif // PROGRAM_H
