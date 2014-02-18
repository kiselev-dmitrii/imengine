#ifndef PICTURE_H
#define PICTURE_H

#include "Widget.h"

namespace imEngine {

/** @brief Абстрактный класс для отображения двумерных текстур
 */
class PictureAbstract : public WidgetAbstract, public BothStretchableAbstract {
public:
        /// Конструктор. Принимает на вход текстуру для отображения
        PictureAbstract(Texture2DPtr texture, WidgetAbstract* parent);

        /// Устанавливает/возвращает текстуру для отображения
        void            setTexture(Texture2DPtr texture);
        Texture2DPtr    texture() const;

        /// Устанавливает размеры виджета
        void            setWidth(float width);
        void            setHeight(float height);

        /// Рендерит текстуру
        virtual void    onRender() = 0;

protected:
        void            initBuffers();

protected:
        Texture2DPtr            m_texture;
        static VertexBufferPtr  s_vbo;
        static VertexArrayPtr   s_vao;
};


/** @brief Класс для отображения двумерных цветовых текстур
 */
class PictureColor : public PictureAbstract {
public:
        /// Конструктор. Принимате на вход цветовую текстуру
        PictureColor(Texture2DPtr texture, WidgetAbstract* parent);

        /// Рендерит текстуру
        void            onRender();

protected:
        void            initProgram();

protected:
        static ProgramPtr       s_program;
};


/** @brief Класс для отображения двумерных текстур глубины
 */
class PictureDepth : public PictureAbstract {
public:
        /// Конструктор, принимает на вход текстуру глубины
        PictureDepth(Texture2DPtr texture, WidgetAbstract* parent);

        /// Устанавливает максимальную и минимальную глубину (используется для приведения)
        void            setNearFarDistances(float near, float far);

        /// Рендерит текстуру
        void            onRender();

protected:
        void            initProgram();

protected:
        float                   m_nearDistance;
        float                   m_farDistance;
        static ProgramPtr       s_program;
};


} //namespace imEngine

#endif // PICTURE_H
