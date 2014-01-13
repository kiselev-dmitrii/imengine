#ifndef PRIMITIVERENDERER_H
#define PRIMITIVERENDERER_H

#include <imEngine/Graphics/GAPI/GAPI.h>
#include <imEngine/System/Window.h>

namespace imEngine {

/** @brief Синглтон предоставляет методы для рендера базовых примитивов:
 *  треугольника, прямоугольника, сферы, куба, параллелепипеда
 */
class PrimitiveRenderer {
public:
        /// Точка досутпа к классу
        static PrimitiveRenderer&      instance();

        /// Отрисовывает прямоугольник в усеченном пространстве
        void    drawRectangleInClipSpace(const Vec2& min, const Vec2& max, const Vec3& color);
        /// Отрисовывает прямоугольник в оконном пространстве
        void    drawRectangleInScreenSpace(const Vec2& pos, const Vec2& size, const Vec3& color, Window* win);

private:
        PrimitiveRenderer();
        PrimitiveRenderer(const PrimitiveRenderer&);
        PrimitiveRenderer& operator =(const PrimitiveRenderer&);

private:
        /// Инициализирует программу
        void            initProgram();
        /// Инициализирует VBO и VAO
        void            initBuffers();

private:
        ProgramPtr      m_program;
        VertexBufferPtr m_vbo;
        VertexArrayPtr  m_vao;
};

} //namespace imEngine

#endif // PRIMITIVERENDERER_H
