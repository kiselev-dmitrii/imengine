#ifndef AABB_H
#define AABB_H

#include "Common.h"

namespace imEngine {

/** @brief Axis Aligned Bound Box.
 */
struct AABB {
        Vec3    min;
        Vec3    max;
};

} //namespace imEngine

#endif // AABB_H
