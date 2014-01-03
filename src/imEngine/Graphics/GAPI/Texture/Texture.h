#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>
#include <memory>
#include <imEngine/Math/Common.h>
#include <imEngine/Utils/Types.h>

namespace imEngine {


/** @brief Определяет тип компоненты пикселя текстуры. ХАРАКТЕРИСТИКА ЗАГРУЖАЕМЫХ ДАННЫХ
 *  Вообщем это тип массива изображения
 */
namespace TextureSrcType {
enum Enum {
        UBYTE                   = GL_UNSIGNED_BYTE,     ///< наиболее популярный
        BYTE                    = GL_BYTE,
        USHORT                  = GL_UNSIGNED_SHORT,
        SHORT                   = GL_SHORT,
        UINT                    = GL_UNSIGNED_INT,
        INT                     = GL_INT,
        FLOAT                   = GL_FLOAT
};
}

/** @brief Определяет как трактуются загружаемые в текстуру данные. ХАРАКТЕРИСТИКА ЗАГРУЖАЕМЫХ ДАННЫХ.
 *  То есть это формат массива изображения. Он определяет лишь взаимное
 *  расположение компонент и их количество на один пиксель.
  */
namespace TextureSrcFormat {
enum Enum {
        RGBA                    = GL_RGBA,      ///< 4 компоненты
        BGRA                    = GL_BGRA,

        RGB                     = GL_RGB,       ///< 3 компоненты
        BGR                     = GL_BGR,

        RG                      = GL_RG,        ///< 2 компоненты

        R                       = GL_RED,       ///< 1 компонента
        DEPTH                   = GL_DEPTH
};
}

/** @brief Внутренний формат текстуры. Задает способ хранения и интерпретации данных.
 *
 *  Множество внутренних форматов делится на группы:
 *      - Нормализованные:
 *              Хранятся в видеопамяти как целые числа, но при чтении переводятся к [0;1] или [-1;1].
 *              Нормализованные беззнаковые будем обозначать через NORM_*, а знаковые - SNORM_*
 *      - Ненормализованные:
 *              Данные хранятся в видеопамяти либо в целочисленном либо во float-point формате.
 *              При чтении преобразование типа не происходит.
 *              Ненормализованые через FLOAT_*, INT_* или UINT_*
 *  Также форматы могут быть цветовыми или depth.
 */
namespace TextureInternalFormat {
enum Enum {
        // Цветовые
        COLOR_NORM_1_COMP_8_BIT                 = GL_RED,
        COLOR_NORM_3_COMP_8_BIT                 = GL_RGB8,
        COLOR_NORM_4_COMP_8_BIT                 = GL_RGBA8,

        // Цветовые float-point
        COLOR_FLOAT_3_COMP_32_BIT               = GL_RGB32F,
        COLOR_FLOAT_4_COMP_32_BIT               = GL_RGBA32F,

        // Depth нормализованные форматы
        DEPTH_NORM_1_COMP_16_BIT                = GL_DEPTH_COMPONENT16,
        DEPTH_NORM_1_COMP_24_BIT                = GL_DEPTH_COMPONENT24,
        DEPTH_NORM_1_COMP_32_BIT                = GL_DEPTH_COMPONENT32,

        // Depth ненормализованные
        DEPTH_FLOAT_1_COMP_32_BIT               = GL_DEPTH_COMPONENT32F
};
}

/** @brief Точка привязки текстуры
 */
namespace TextureTarget {
enum Enum {
        TEXTURE_1D              = GL_TEXTURE_1D,
        TEXTURE_2D              = GL_TEXTURE_2D,
        TEXTURE_3D              = GL_TEXTURE_3D,
        CUBEMAP                 = GL_TEXTURE_CUBE_MAP
};
}


/** @brief Метод интерполяции, при уменьшении текстуры
 */
namespace TextureMinFilter {
enum Enum {
        NEAREST                         = GL_NEAREST,
        LINEAR                          = GL_LINEAR,
        NEAREST_MIPMAP_NEAREST          = GL_NEAREST_MIPMAP_NEAREST,
        LINEAR_MIPMAP_NEAREST           = GL_LINEAR_MIPMAP_NEAREST,
        NEAREST_MIPMAP_LINEAR           = GL_NEAREST_MIPMAP_LINEAR,
        LINEAR_MIPMAP_LINEAR            = GL_LINEAR_MIPMAP_LINEAR
};
}


/** @brief Метод интерполяции, при увеличении текстуры
 */
namespace TextureMagFilter {
enum Enum {
        NEAREST                         = GL_NEAREST,
        LINEAR                          = GL_LINEAR
};
}


/** @brief Режим накладывания текстуры на полигон
 */
namespace TextureWrapMode {
enum Enum {
        REPEAT                  = GL_REPEAT,            ///< текстура повторяется
        REPEAT_MIRRORED         = GL_MIRRORED_REPEAT,   ///< текстура при повторении отражается
        CLAMP_TO_EDGE           = GL_CLAMP_TO_EDGE,     ///< граниченые пиксели текстуры растягиваются
        CLAMP_TO_BORDER         = GL_CLAMP_TO_BORDER    ///< граница заливается цветом border'а текстуры
};
}


/** @brief Перечисление функций сравнения
  *
  * В GLSL команда textureProj(sampler, coord) для sampler*Shadow эквивалентна:
  * cmp(texture2DProj(sampler, coord.xy).r, coord.z) ? 1 : 0
  * Данная команда служит для установки конкретного оператора сравнения
  */
namespace TextureCompareFunction {
enum Enum {
        LESS_OR_EQUAL                   = GL_LEQUAL,     //cmp = '<='
        LESS                            = GL_LESS,       //cmp = '<'

        GREATER_OR_EQUAL                = GL_GEQUAL,     //cmp = '>='
        GREATER                         = GL_GREATER,    //cmp = '>'

        EQUAL                           = GL_EQUAL,      //cmp = '=='
        NOT_EQUAL                       = GL_NOTEQUAL,   //cmp = '!='

        ALWAYS                          = GL_ALWAYS,     //cmp = 'true'
        NEVER                           = GL_NEVER       //cmp = 'false'
};
}


/** @brief Режим сравнения. Актуально для текстур глубины
 */
namespace TextureCompareMode {
enum Enum {
        REF_TO_TEXTURE                  = GL_COMPARE_REF_TO_TEXTURE,
        NONE                            = GL_NONE
};
}


/** @brief Базовый класс для работы с текстурами.
 *
 *  Предполагается, что контекст OpenGL уже создан.
 *  Текстура может загружать себя из файла с помощью библиотеки DevIL
 */
class Texture {
public:
        /// Конструктор. Создает текстуру и привязывает ее к таргету
        Texture(TextureTarget::Enum target);
        /// Деструктор. Уничтожает текстуру
        virtual ~Texture();

        /// Загружает текстуру из файла
        virtual void                    load(const String&)                     {}

        /// Возвращает OGL дескриптор текстуры
        GLuint                          handle();
        /// Возвращает точку привязки текстуры
        TextureTarget::Enum             target();

        /// Возвращает ширину текстуры
        int                             width();
        /// Возвращает высоту текстуры
        int                             height();
        /// Возвращает глубину
        int                             depth();
        /// Возвращает количество изображений в текстуре
        uint                            numberOfImages();

        /// Возвращает количество каналов (количество компонент на пиксель)
        int                             numberOfChannels();
        /// Возвращает размер одной компоненты
        int                             sizeOfComponent();
        /// Возвращает размер текстуры в байтах
        int                             sizeOfData();

        /// Определяет была ли выделена видеопамять под текстуру.
        bool                            wasMemoryAllocated();

        /// Возвращает внутренний формат текстуры. Неопределено, если wasMemoryAllocated() == false
        TextureInternalFormat::Enum     internalFormat();
        /// Возвращает формат данных текстуры (взаимное расположение компонент в пикселе и их число)
        TextureSrcFormat::Enum          sourceFormat();
        /// Возвращает тип данных текстуры
        TextureSrcType::Enum            sourceType();

        /// Установка интерполяции при увеличении
        void                            setMagnificationFilter(TextureMagFilter::Enum filter);
        /// Установка интерполяции при уменьшении
        void                            setMinimizationFilter(TextureMinFilter::Enum filter);

        /// Установка режима накладывания текстуры
        void                            setWrap(TextureWrapMode::Enum mode);
        /// Установка режима накладывания текстуры по координате S
        void                            setWrapS(TextureWrapMode::Enum mode);
        /// Установка режима накладывания текстуры по координате T
        void                            setWrapT(TextureWrapMode::Enum mode);
        /// Установка режима накладывания текстуры по координате R
        void                            setWrapR(TextureWrapMode::Enum mode);

        /// Опредеяет наименьший уровень мипмапа
        void                            setMinMipLevel(uint min);
        /// Опеределяет наибольший уровень мипмапа
        void                            setMaxMipLevel(uint max);

        /// Устанавилвает режим сравнения (актуально для ShadowMap);
        void                            setCompareMode(TextureCompareMode::Enum mode);
        /// Устанавилвает функцию сравнения (актуально для ShadowMap)
        void                            setCompareFunction(TextureCompareFunction::Enum func);

        /// Устанавливает цвет границы текстуры (полезно например при CLAMP_TO_BORDER)
        void                            setBorderColor(const Vec4& color);

        /// Генерирует mip-уровни
        void                            generateMipmaps();

        /// Привязывает текстуру к текстурному блоку с номером unit
        /// Если unit = -1, то привязывает к текущему активному текстурному блоку
        void                            bind(int unit = -1);
        /// Отвязывает текстуру
        void                            unbind();

        /// Возвращает данные внутри текстуры.
        ///Медленный метод, поскольку требуется синхронизация GPU и CPU
        std::shared_ptr<ubyte>          data();

protected:
        /// Метод служит для того, чтобы обновлять информацию о текстуре после ее загрузки
        void                            updateTextureInformation(GLsizei width, GLsizei height, GLsizei depth, uint numberOfImages,  TextureInternalFormat::Enum internalFormat,
                                                                 TextureSrcType::Enum srcType, TextureSrcFormat::Enum srcFormat, bool wasMemoryAllocated);

protected:
        GLuint                          m_handle;
        TextureTarget::Enum             m_target;

        // Параметры, которые должны быть установлены после загрузки
        GLsizei                         m_width;
        GLsizei                         m_height;
        GLsizei                         m_depth;
        uint                            m_numberOfImages;         //количество изображений (например в cupemap их 6)
        TextureInternalFormat::Enum     m_internalFormat;
        TextureSrcType::Enum            m_srcType;
        TextureSrcFormat::Enum          m_srcFormat;
        bool                            m_wasMemoryAllocated;   //была ли выделена память под текстуру

        static GLuint                   s_boundHandle;
        static int                      s_currentUnit;
};


} //namespace imEngine

#endif // TEXTURE_H
