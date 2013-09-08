#ifndef GLERRORMONITOR_H
#define GLERRORMONITOR_H

#include <GL/glew.h>
#include <imCore/Utils/Types.h>

namespace imCore {

/** @brief Статический класс, который дает возможность получать информацию об ошибках OpenGL
 */
class GLErrorMonitor {
public:
        /// Преобразование OpenGL перечисления в его строковый эквивалент
        static String   glEnumToString(GLenum en);
};

} //namespace imCore

#endif // GLERRORMONITOR_H
