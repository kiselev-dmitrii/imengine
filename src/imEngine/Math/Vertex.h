#ifndef VERTEX_H
#define VERTEX_H

#include "Common.h"

namespace imEngine {

/** @brief Вершина - структура, хранящая в себе позицию, нормаль и текстурные координаты
 */
struct Vertex {
        Vertex() : position(0), normal(0), texcoords(0), tangent(0) { }
        Vertex(Vec3 p, Vec3 n, Vec2 tex) : position(p), normal(n), texcoords(tex), tangent(0) { }

        Vec3    position;
        Vec3    normal;
        Vec2    texcoords;
        Vec3    tangent;
};

} //namespace imEngine

#endif // VERTEX_H
