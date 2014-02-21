#ifndef SKYBOX_H
#define SKYBOX_H

#include <imEngine/Graphics/GAPI/GAPI.h>
#include <imEngine/Graphics/Geometry.h>

namespace imEngine {


/** @brief Скайбокс
 */
class Skybox {
public:
        /// Конструктор
        explicit Skybox(const String& filename);
        explicit Skybox(const CubeTexturePtr texture);

        /// Устанавливает/возвращает текстуру окружения
        void            setTexture(CubeTexturePtr texture);
        CubeTexturePtr  texture() const;

        /// Рендерит скайбокс.
        /// viewRotationMatrix - это матрица 3x3 - Mat3(viewMatrix)
        /// projectionMatrix - проекционная матрица
        void            render(const Mat3& viewRotationMatrix, const Mat4& projectionMatrix);

private:
        CubeTexturePtr  m_texture;
        GeometryPtr     m_cube;
        Program*        m_program;
};


/** @brief Умный указатель для Skybox
 */
typedef std::shared_ptr<Skybox> SkyboxPtr;


} //namespace imEngine

#endif // SKYBOX_H
