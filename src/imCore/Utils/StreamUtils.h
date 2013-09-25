#ifndef STREAMUTILS_H
#define STREAMUTILS_H

#include <iostream>
#include "../Math/Common.h"
#include "Types.h"

namespace imCore {

/** @brief Наборы функций и операторов для удобного ввода-вывода
  */
namespace StreamUtils {
        std::ostream &operator<<(std::ostream &stream, const IVec2 &vec);
        std::ostream &operator<<(std::ostream &stream, const Vec2 &vec);
        std::ostream &operator<<(std::ostream &stream, const Vec3 &vec);
        std::ostream &operator<<(std::ostream &stream, const Vec4 &vec);
        std::ostream &operator<<(std::ostream &stream, const Mat3 &mat);
        std::ostream &operator<<(std::ostream &stream, const Mat4 &mat);
        std::ostream &operator<<(std::ostream &stream, const Quat &q);

        std::ostream &operator<<(std::ostream &stream, const StringList &stringList);

} //namespace StreamUtils

} //namespace imCore

#endif // STREAMUTILS_H
