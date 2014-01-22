#ifndef PICTURE_H
#define PICTURE_H

#include "Widget.h"

namespace imEngine {

/** @brief Класс для отображения двумерных текстур
 */
class Picture : public WidgetAbstract, public BothStretchableAbstract {
public:
        /// Конструктор
        /// Принимает на вход текстуру для отображения
        Picture(Texture2DPtr texture, WidgetAbstract* parent);

        /// Устанавливает размеры виджета
        void    setWidth(float width);
        void    setHeight(float height);

        /// Рендерит текстуру
        void    onRender();

private:
        void    initProgram();
        void    initBuffers();

private:
        Texture2DPtr            m_texture;

        static ProgramPtr       s_program;
        static VertexBufferPtr  s_vbo;
        static VertexArrayPtr   s_vao;

};

} //namespace imEngine

#endif // PICTURE_H
