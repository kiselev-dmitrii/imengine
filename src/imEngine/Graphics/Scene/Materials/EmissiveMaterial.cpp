#include "EmissiveMaterial.h"
#include <imEngine/Utils/StringUtils.h>

namespace imEngine {

EmissiveMaterial::EmissiveMaterial() :
        EntityMaterial("materials/EmissiveMaterial.glsl", MaterialType::UNLIGHTENED)
{
        setEmissiveColor(Vec3(1.0));
}

void EmissiveMaterial::loadFromJson(const JsonValue &value) {
        JsonValue emissiveColor = value["emissiveColor"];

        if (!emissiveColor.isNull()) setEmissiveColor(JsonUtils::toVec3(emissiveColor));
}

void EmissiveMaterial::bind() {
        m_program->bind();
        m_program->setUniform("uIntensity", m_emissiveColor);
}

void EmissiveMaterial::unbind() {
        m_program->unbind();
}

} //namespace imEngine
