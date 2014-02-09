#include "Volume.h"
#include "../Scene.h"
#include <imEngine/Utils/Debug.h>

namespace imEngine {

ProgramPtr Volume::s_program;

Volume::Volume(Texture3D *texture, SceneObject *parent) :
        Entity(parent),
        m_cube(Geometry::cube()),
        m_texture(texture)
{
        initProgram();
}

void Volume::render() {
        IM_TODO;
}

void Volume::initProgram() {
        static bool wasInited = false;
        if (wasInited) return;

        s_program = ProgramPtr(new Program());
        s_program->loadSource("resources/shaders/VolumeRender.glsl");
        s_program->build();

        wasInited = true;
}



} //namespace imEngine

