#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <imEngine/Graphics/GAPI/GAPI.h>
#include <imEngine/Math/AABB.h>
#include <imEngine/FileContainers/Mesh.h>

namespace imEngine {


/** @brief Умный указатель для Geometry
 */
class Geometry;
typedef std::shared_ptr<Geometry> GeometryPtr;


/** @brief Структура для хранения полигональной фигуры в видео памяти
 *
 *  При рендере геометрии шейдер должен иметь на входе 3 аттрибута.
 *  @code
 *      layout (location = 0) in vec3 vPosition;
 *      layout (location = 1) in vec3 vNormal;
 *      layout (location = 2) in vec2 vTexcoords;
 *  @endcode
 */
class Geometry {
public:
        /// Конструктор. Загружает индексы и вершины в видеопамять
        explicit Geometry(const VertexList& vertices, const IndexList& indices);
        /// Конструктор. Загружает меш в видеопамять
        explicit Geometry(const Mesh& mesh);

        /// Возвращает описывающий прямоугольник
        const AABB&     aabb() const;
        /// Возвращает центральную точку фигуры
        const Vec3&     center() const;
        /// Возвращает радиус (расстояние от (0,0,0) до максимальной точки)
        float           radius() const;

        /// Возвращает из видеопамяти список вершин/индексов
        VertexList      vertices();
        IndexList       indices();

        /// Применяет матрицу m для фигуры. Долгая операция
        void            transform(const Mat4& matrix);

        /// Загружает меш в буферы
        void            load(const VertexList& vertices, const IndexList& indices);
        void            load(const Mesh& mesh);

        /// Отрисовывает фигуру. При этом какой-либо шейдер должен быть установлен
        void            render() const;

public:
        /// Создает геометрию куба [-1;1]x[-1;1]x[-1;1]
        static GeometryPtr      cube();
        /// Создает параллелепипед с мин. координатой min и максимальной max
        static GeometryPtr      box(const Vec3& min, const Vec3& max);
        /// Создает геометрию плоскости на [-1;1]x[-1;1]x[0;0]
        static GeometryPtr      plane();

private:
        /// Инициализирует и настраивает буферы
        void            initBuffers();
        /// Рассчитывает и кэширует различную информацию о геометрии
        void            calculateGeometryInformation();

private:
        VertexArrayPtr  m_vao;
        VertexBufferPtr m_vbo;
        IndexBufferPtr  m_ibo;

        uint            m_numVertices;
        uint            m_numIndices;

        bool            m_areVerticesLoaded;

        AABB            m_aabb;
        Vec3            m_center;
        float           m_radius;
};


} //namespace imEngine

#endif // GEOMETRY_H
