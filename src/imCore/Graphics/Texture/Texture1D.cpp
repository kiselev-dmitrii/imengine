#include "Texture1D.h"
#include "../../Utils/Debug.h"
#include "../../Utils/GLUtils.h"

namespace imCore {

Texture1D::Texture1D(TextureInternalFormat::Enum internal, int width) : Texture(TextureTarget::TEXTURE_1D) {
        m_width = width;
        IM_TODO;
}

Texture1D::Texture1D(TextureInternalFormat::Enum internal, int width, TextureSrcType::Enum srcType,
                     TextureSrcFormat::Enum srcFormat, GLvoid *src) : Texture(TextureTarget::TEXTURE_1D) {
        m_width = width;
        IM_TODO;
}

Texture1D::Texture1D(const String &path) : Texture(TextureTarget::TEXTURE_1D){
        IM_TODO;
}

void Texture1D::upload(TextureInternalFormat::Enum internal, int width, TextureSrcType::Enum srcType, TextureSrcFormat::Enum srcFormat, GLvoid *src) {
        bind();
        IM_GLCALL(glTexImage1D(m_target, 0, internal, width, 0, srcFormat, srcType, src));
        m_width = width;

        IM_LOG(GLUtils::convertEnumToString(m_target) << " texture with handle " << m_handle << " uploaded");
}

void Texture1D::upload(const String &path) {
        bind();
}

} //namespace imCore
