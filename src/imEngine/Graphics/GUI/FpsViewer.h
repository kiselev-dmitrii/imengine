#ifndef FPSVIEWER_H
#define FPSVIEWER_H

#include "Text.h"

namespace imEngine {


/** @brief Служит для отображения текущего FPS
 */
class FpsViewer : public Text {
public:
        /// Конструктор
        FpsViewer(WidgetAbstract* parent);

        /// Считает количество кадров и каждую секунду обновляет FPS
        void    onUpdate();

private:
        uint    m_fps;
        float   m_prevTime;
};


} //namespace imEngine

#endif // FPSVIEWER_H
