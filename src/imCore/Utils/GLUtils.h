#ifndef GLUTILS_H
#define GLUTILS_H

#include <GL/glew.h>
#include "Types.h"

namespace imCore {

/** @brief Различные утилиты для OpenGL
 */
namespace GLUtils {
        /// Конвертирует тип GLenum в соответствуюшую строку
        String          convertEnumToString(GLenum enumeration);

} //namespace GLUtils

} //namespace imCore

#endif // GLUTILS_H
