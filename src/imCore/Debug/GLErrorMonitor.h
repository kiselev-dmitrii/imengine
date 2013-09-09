#ifndef GLERRORMONITOR_H
#define GLERRORMONITOR_H

#include <GL/glew.h>
#include <imCore/Utils/Types.h>

namespace imCore {

/** @brief Статический класс, который дает возможность получать информацию об ошибках OpenGL
 */
class GLErrorMonitor {
public:
        /// Возвращает последнуюю ошибку OpenGL
        static GLenum   lastError();

        /// Преобразование OpenGL перечисления в его строковый эквивалент
        static String   glEnumToString(GLenum enumeration);
};

} //namespace imCore

#endif // GLERRORMONITOR_H
