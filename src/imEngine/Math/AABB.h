#ifndef AABB_H
#define AABB_H

#include "Common.h"

namespace imEngine {

/** @brief Axis Aligned Bound Box.
 */
struct AABB {
        Vec3    min;
        Vec3    max;

        /// Определяет, находится ли точка point внутри AABB
        bool doesContain(const Vec3& point) const {
                return glm::all(glm::greaterThan(point, min)) &&
                       glm::all(glm::lessThan(point, max));
        }
};

} //namespace imEngine

#endif // AABB_H
