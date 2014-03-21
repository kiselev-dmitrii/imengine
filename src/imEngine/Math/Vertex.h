#ifndef VERTEX_H
#define VERTEX_H

#include "Common.h"

namespace imEngine {

/** @brief Вершина - структура, хранящая в себе позицию, нормаль и текстурные координаты
 */
struct Vertex {
        Vertex() : position(0), normal(0), tangent(0), texcoords(0) { }
        Vertex(Vec3 p, Vec3 n, Vec2 tex) : position(p), normal(n), tangent(0), texcoords(tex) { }

        Vec3    position;
        Vec3    normal;
        Vec3    tangent;
        Vec2    texcoords;
};

} //namespace imEngine

#endif // VERTEX_H
