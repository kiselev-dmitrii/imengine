#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <imEngine/Graphics/GAPI/GAPI.h>
#include <imEngine/Math/AABB.h>
#include <imEngine/FileContainers/Mesh.h>

namespace imEngine {


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
        /// Конструктор. Загружает меш в видеопамять
        explicit Geometry(const Mesh& mesh);

        /// Возвращает описывающий прямоугольник
        const AABB&     aabb() const;
        /// Возвращает центральную точку фигуры
        const Vec3&     center() const;
        /// Возвращает радиус (расстояние от (0,0,0) до максимальной точки)
        float           radius() const;

        /// Применяет матрицу m для фигуры. Долгая операция
        void            transform(const Mat4& matrix);

        /// Загружает меш в буферы
        void            load(const Mesh& mesh);

        /// Отрисовывает фигуру. При этом какой-либо шейдер должен быть установлен
        void            render() const;

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
