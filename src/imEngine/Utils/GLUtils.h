#ifndef GLUTILS_H
#define GLUTILS_H

#include <GL/glew.h>
#include "Types.h"

namespace imEngine {

/** @brief Различные утилиты для OpenGL
 */
namespace GLUtils {

        /// Возвращает размер типа в байтах для OGL типа
        uint            sizeOf(GLenum glType);
        /// Конвертирует тип GLenum в соответствуюшую строку
        String          convertEnumToString(GLenum enumeration);

} //namespace GLUtils

} //namespace imEngine

#endif // GLUTILS_H
