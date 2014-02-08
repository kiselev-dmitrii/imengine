#include "TextureMaterial.h"
#include <imEngine/Utils/Debug.h>
#include <imEngine/Graphics/Scene/Resources.h>

namespace imEngine {

TextureMaterial::TextureMaterial(Texture2D* texture) {
        initProgram("resources/shaders/materials/TextureMaterial.glsl");
        setTexture(texture);
}

void TextureMaterial::setTexture(Texture2D* texture) {
        m_textureBase = texture;
        m_textureBase->setMagnificationFilter(TextureMagFilter::LINEAR);
        m_textureBase->setMinimizationFilter(TextureMinFilter::LINEAR);
}

Texture2D* TextureMaterial::texture() const {
        return m_textureBase;
}

void TextureMaterial::bind() {
        s_program->bind();

        m_textureBase->bind(0);
        s_program->setUniform("uTextureBase", 0);
}

void TextureMaterial::unbind() {
        s_program->unbind();
        m_textureBase->unbind();
}

} //namespace imEngine
