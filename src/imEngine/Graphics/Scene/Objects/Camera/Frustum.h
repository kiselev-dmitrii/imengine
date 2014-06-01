#ifndef FRUSTUM_H
#define FRUSTUM_H

#include <imEngine/Math/Common.h>
#include <imEngine/Math/Plane.h>
#include <imEngine/FileContainers/Json.h>

namespace imEngine {


/** @brief Усеченная пирамида
 */
class Frustum {
public:
        /// Конструктор
        Frustum();

        /// Загружает настройки из json-ноды
        void            loadFromJson(const JsonValue& node);

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

        /// Конструирует и возвращает матрицу перехода от видовых координат к усеченным
        /// Любой точка в внутри пирамиды будет преобразована в точку в промужетку [-1;1]
        const Mat4&     viewToClipMatrix();

private:
        /// Обновляет матрицу m_viewToClipMatrix, если это необходимо
        void            updateViewToClipMatrix();

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
