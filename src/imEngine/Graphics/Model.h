#ifndef MODEL_H
#define MODEL_H

#include <imEngine/Graphics/GAPI/GAPI.h>

namespace imEngine {


/** @brief Составная часть полигональной модели.
 *
 * Содержит геометрию в виде VBO, IBO, VAO, а также материал с которым будет
 * производиться визуализация.
 * Также модель имеет шейдер, посредством которого происходит визуализация
 */
class ModelDetail {
public:
private:

        VertexBufferPtr m_vbo;
        IndexBufferPtr  m_ibo;
        VertexArrayPtr  m_vao;

//      MaterialPtr     m_material;
        ProgramPtr      m_program;
};


/** @brief Представляет собой полигональную модель в видеопамяти.
 *
 *  Фактически является набором из VAO, VBO и IBO.
 *  Имеет метод render(), для отрисовки объекта текущим шейдером.
 *  @todo
 *      - Реализовать
 */
class Model {
public:
        Model();

private:
        typedef std::vector<ModelDetail> ModelDetailList;
        ModelDetailList m_details;
};


} //namespace imEngine

#endif // MODEL_H
