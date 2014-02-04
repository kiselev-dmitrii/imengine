#ifndef PLANE_H
#define PLANE_H

#include "Common.h"

namespace imEngine {


/** @brief Плоскость
 */
class Plane {
public:
        /// Конструктор. Создает плоскость расположенную в (0,0,0) и ориентированную в +z
        Plane();
        /// Конструктор. Создает плоскость по коэффициентам ax+by+cz+d = 0;
        Plane(float a, float b, float c, float d);
        /// Конструктор. Создает плоскость по 3 компланарным точкам
        Plane(const Vec3& p1, const Vec3& p2, const Vec3& p3);
        /// Создает точку по положению и нормали
        Plane(const Vec3& origin, const Vec3& normal);

        /// Устанавливает/возвращает нормаль
        void    setNormal(const Vec3& normal);
        Vec3    normal() const;
        /// Устанавливает/возвращает D
        void    setOffset(float d);
        float   offset() const;
        /// Устанавливает/возвращает положение
        void    setOrigin(const Vec3& origin);
        Vec3    origin() const;

        /// Возвращает дистанцию от точки point плоскости
        float   distance() const;
        /// Возвращает ближайшую к point точку на плоскости
        Vec3    closest(const Vec3& point) const;

        /// Преобразовывает все точки с помощью матрицы matrix
        void    transform(const Mat4& matrix);

private:
        Vec3    m_normal;
        float   m_offset;
};


} //namespace imEngine

#endif // PLANE_H
