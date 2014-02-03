#ifndef CAMERA_H
#define CAMERA_H

namespace imEngine {

/** @brief Абстрактный класс для камеры
 */
class CameraAbstract {
public:
        CameraAbstract();
};


/** @brief Камера
 */
class FirstPersonCamera : public CameraAbstract {
public:
        FirstPersonCamera();
};


/** @brief Камера, постоянно направленная на некоторую цель
 */
class TargetCamera : public CameraAbstract {
        TargetCamera();
};

} //namespace imEngine

#endif // CAMERA_H
