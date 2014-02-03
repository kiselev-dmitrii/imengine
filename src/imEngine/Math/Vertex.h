#ifndef VERTEX_H
#define VERTEX_H

#include "Common.h"

namespace imEngine {

/** @brief Вершина - структура, хранящая в себе позицию, нормаль и текстурные координаты
 */
struct Vertex {
        Vec3    position;
        Vec3    normal;
        Vec2    texcoords;
};

} //namespace imEngine

#endif // VERTEX_H
