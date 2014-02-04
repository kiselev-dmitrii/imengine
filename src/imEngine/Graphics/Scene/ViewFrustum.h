#ifndef FRUSTUM_H
#define FRUSTUM_H

#include <imEngine/Math/Common.h>

namespace imEngine {


/** @brief Сторона пирамиды
 */
enum class FrustumSide {
        TOP,
        BOTTOM,
        NEAR,
        FAR,
        LEFT,
        RIGHT
};


/** @brief Усеченная пирамида
 */
class Frustum {
public:
        /// Конструктор
        Frustum();

        /// Устанавливает/определяет ортогональный тип проекции
        void            setOrthographic(bool isOrthographic);
        bool            isOrthographic() const;
        /// Устанавливает/возвращает расстояние до ближней отсекающей плоскости
        void            setNearDistance(float distance);
        float           nearDistance() const;
        /// Устанавливает/возвращает расстояние до дальней отсекающей плоскости
        void            setFarDistance(float distance);
        float           farDistance() const;
        /// Устанавливает/возаращает соотношение сторон
        void            setAspectRatio(float value);
        float           aspectRatio() const;
        /// Устанавливает/возвращает угол обзора в градусах
        void            setFieldOfView(float angle);
        float           fieldOfView() const;

        /// Устанавливает матрицу. В результате чего изменяются параметры
        void            setMatrix(const Mat4& matrix);
        /// Возвращает плоскость пирамиды
        const Plane&    plane(FrustumSide side) const;

        /// Определяет, лежит ли точка/сфера внутри пирамиды
        bool            isInside(Vec3& vsPoint);
        bool            isInside(Vec3& vsOrigin, float radius);

        /// Конструирует и возвращает матрицу перехода от видовых координат к усеченным
        const Mat4&     viewToClipMatrix() const;

private:
        bool            m_isOrthographic;
        float           m_nearDistance;
        float           m_farDistance;
        float           m_aspectRatio;
        float           m_fieldOfView;

        Mat4            m_viewToClipMatrix;
        bool            m_isNeedToUpdateMatrix;
};


} //namespace imEngine

#endif // FRUSTUM_H
