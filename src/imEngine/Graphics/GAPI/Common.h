#ifndef GAPICOMMON_H
#define GAPICOMMON_H

#include <GL/glew.h>

namespace imEngine {

/** @brief Определяет тип компоненты пикселя текстуры. ХАРАКТЕРИСТИКА ЗАГРУЖАЕМЫХ ДАННЫХ
 *  Вообщем это тип массива изображения
 */
namespace SourceType {
        enum Enum {
                UBYTE                   = GL_UNSIGNED_BYTE,     ///< наиболее популярный
                BYTE                    = GL_BYTE,
                USHORT                  = GL_UNSIGNED_SHORT,
                SHORT                   = GL_SHORT,
                UINT                    = GL_UNSIGNED_INT,
                INT                     = GL_INT,
                FLOAT                   = GL_FLOAT
        };

        int sizeOf(SourceType::Enum type);
}


/** @brief Определяет как трактуются загружаемые в текстуру данные. ХАРАКТЕРИСТИКА ЗАГРУЖАЕМЫХ ДАННЫХ.
 *  То есть это формат массива изображения. Он определяет лишь взаимное
 *  расположение компонент и их количество на один пиксель.
  */
namespace SourceFormat {
        enum Enum {
                RGBA                    = GL_RGBA,              ///< 4 компоненты
                BGRA                    = GL_BGRA,

                RGB                     = GL_RGB,               ///< 3 компоненты
                BGR                     = GL_BGR,

                RG                      = GL_RG,                ///< 2 компоненты

                R                       = GL_RED,               ///< 1 компонента
                DEPTH                   = GL_DEPTH_COMPONENT
        };

        int numComponents(SourceFormat::Enum srcFormat);
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
namespace InternalFormat {
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


} //namespace imEngine

#endif // GAPICOMMON_H
