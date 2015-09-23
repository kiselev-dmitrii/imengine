#ifndef MESH_H
#define MESH_H

#include <imEngine/Math/Vertex.h>
#include <imEngine/Utils/Types.h>
#include <assimp/mesh.h>

namespace imEngine {

typedef std::vector<Vertex> VertexList;
typedef std::vector<uint> IndexList;

/** @brief Полигональный объект. Представляет собой набор вершин и индексов.
 *
 * Используется для получения вершинных данных из файлов .obj, .3ds и т.д
 * Для загрузки меша из файла используется библиотека Assimp.
 */
class Mesh {
public:
        /// Конструктор. Загружает меш из файла
        explicit Mesh(const String& filename);

        /// Загружает меш из файла filename
        void                    load(const String& filename);

        /// Возвращает вершины/индексы
        const VertexList&       vertices() const;
        const IndexList&        indices() const;

private:
        IndexList               loadIndices(const aiMesh* mesh);
        VertexList              loadVertices(const aiMesh* mesh);

private:
        VertexList      m_vertices;
        IndexList       m_indices;
};


} //namespace imEngine

#endif // MESH_H
