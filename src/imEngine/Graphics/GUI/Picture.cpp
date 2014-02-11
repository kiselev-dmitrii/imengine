#include "Picture.h"
#include "PictureProgram.glsl"

namespace imEngine {

ProgramPtr Picture::s_program;
VertexBufferPtr Picture::s_vbo;
VertexArrayPtr Picture::s_vao;

Picture::Picture(Texture2DPtr texture, WidgetAbstract *parent) :
        WidgetAbstract(parent),
        BothStretchableAbstract(),
        m_texture(texture)
{
        m_size = Vec2(texture->width(), texture->height());
        initProgram();
        initBuffers();
}

void Picture::setTexture(Texture2DPtr texture) {
        m_texture = texture;
}

Texture2DPtr Picture::texture() const {
        return m_texture;
}

void Picture::setWidth(float width) {
        if (width < minimalSize().x) return;

        m_size.x = width;
        onWidthChange();
}

void Picture::setHeight(float height) {
        if (height < minimalSize().y) return;

        m_size.y = height;
        onHeightChange();
}

void Picture::onRender() {
        // Биндим все переменные
        s_program->bind();
        s_program->setUniform("u_windowSize", Vec2(manager()->window()->size()));
        s_program->setUniform("u_position", absolutePosition());
        s_program->setUniform("u_size", size());
        m_texture->bind(0);
        s_program->setUniform("u_texture", 0);
        s_program->setUniform("u_opacity", opacity());

        // Рендерим
        s_vao->bind();
        Renderer::renderVertices(Primitive::POINT, 1);

        // Рендерим детей
        renderChildren();
}

void Picture::initProgram() {
        static bool wasInited = false;
        if (wasInited) return;

        s_program = ProgramPtr(new Program());
        s_program->loadSource(pictureProgramSource);
        s_program->build();

        wasInited = true;
}

void Picture::initBuffers() {
        static bool wasInited = false;
        if (wasInited) return;

        s_vbo = VertexBufferPtr(new VertexBuffer());
        s_vao = VertexArrayPtr(new VertexArray());
        s_vao->bind();
                s_vbo->connect(0, 2, GL_FLOAT, 0, 0);
        s_vao->unbind();

        Vec2 data(0,0);                 //всего одна точка. положение будет регулироваться юниформами
        s_vbo->load(&data, sizeof(data), BufferUsage::STATIC_DRAW);

        wasInited = true;
}

} //namespace imEngine
