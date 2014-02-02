#ifndef MESH_H
#define MESH_H

#include <imEngine/Math/Common.h>
#include <imEngine/Utils/Types.h>

namespace imEngine {

/** @brief Структура для хранения вершины меша.
 */
struct MeshVertex {
        Vec3    position;
        Vec3    normal;
        Vec2    texcoord;
};

/** @brief Полигональный объект. Представляет собой набор вершин и индексов.
 *
 * Для загрузки меша из файла используется библиотека Assimp.
 * Предполагается, что меш хранит в себе объект лишь с одним материалом.
 */
class Mesh {
public:
        /// Конструктор. Загружает меш из файла
        Mesh(const String& filename);
};

} //namespace imEngine

#endif // MESH_H
