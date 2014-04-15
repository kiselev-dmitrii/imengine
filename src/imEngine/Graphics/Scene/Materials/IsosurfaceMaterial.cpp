#include "IsosurfaceMaterial.h"

namespace imEngine {

IsosurfaceMaterial::IsosurfaceMaterial() :
        VolumeMaterial("materials/IsosurfaceMaterial.glsl")
{
        setThresholdDensity(0.3);
}

void IsosurfaceMaterial::bind() {
        VolumeMaterial::bind();
        m_program->setUniform("uThresholdDensity", m_thresholdDensity);
}

} //namespace imEngine
