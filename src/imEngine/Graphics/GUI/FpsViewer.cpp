#include "FpsViewer.h"
#include <imEngine/Application/GraphicApplication.h>

namespace imEngine {

FpsViewer::FpsViewer(WidgetAbstract* parent) :
        Text("FPS: 0", parent),
        m_fps(0),
        m_prevTime(manager()->application()->currentTime())
{ }

void FpsViewer::onUpdate() {
        ++m_fps;
        if ((manager()->application()->currentTime() - m_prevTime) >= 1.0) {
                setText("FPS: " + std::to_string(m_fps));
                m_fps = 0;
                m_prevTime = manager()->application()->currentTime();
        }
}

} //namespace imEngine
