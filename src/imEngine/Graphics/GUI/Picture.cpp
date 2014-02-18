#include "Picture.h"
#include "PictureProgram.glsl"

namespace imEngine {

//############################# PictureAbstract ##############################//

VertexBufferPtr PictureAbstract::s_vbo;
VertexArrayPtr PictureAbstract::s_vao;

PictureAbstract::PictureAbstract(Texture2DPtr texture, WidgetAbstract *parent) :
        WidgetAbstract(parent),
        BothStretchableAbstract(),
        m_texture(texture)
{
        m_size = Vec2(texture->width(), texture->height());
        initBuffers();
}

void PictureAbstract::setTexture(Texture2DPtr texture) {
        m_texture = texture;
}

Texture2DPtr PictureAbstract::texture() const {
        return m_texture;
}

void PictureAbstract::setWidth(float width) {
        if (width < minimalSize().x) return;

        m_size.x = width;
        onWidthChange();
}

void PictureAbstract::setHeight(float height) {
        if (height < minimalSize().y) return;

        m_size.y = height;
        onHeightChange();
}

void PictureAbstract::initBuffers() {
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

//############################# PictureColor ##############################//

ProgramPtr PictureColor::s_program;

PictureColor::PictureColor(Texture2DPtr texture, WidgetAbstract *parent) :
        PictureAbstract(texture, parent)
{
        initProgram();
}

void PictureColor::onRender() {
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

void PictureColor::initProgram() {
        static bool wasInited = false;
        if (wasInited) return;

        s_program = ProgramPtr(new Program());
        s_program->loadSource(pictureProgramSource);
        s_program->build();

        wasInited = true;
}

//############################# PictureDepth ##############################//

ProgramPtr PictureDepth::s_program;

PictureDepth::PictureDepth(Texture2DPtr texture, WidgetAbstract *parent) :
        PictureAbstract(texture, parent),
        m_nearDistance(0.05),
        m_farDistance(100.0)
{
        initProgram();
}

void PictureDepth::setNearFarDistances(float near, float far) {
        m_nearDistance = near;
        m_farDistance = far;
}

void PictureDepth::onRender() {
        // Биндим все переменные
        s_program->bind();
        s_program->setUniform("u_windowSize", Vec2(manager()->window()->size()));
        s_program->setUniform("u_position", absolutePosition());
        s_program->setUniform("u_size", size());
        m_texture->bind(0);
        s_program->setUniform("u_texture", 0);
        s_program->setUniform("u_nearDistance", m_nearDistance);
        s_program->setUniform("u_farDistance", m_farDistance);
        s_program->setUniform("u_opacity", opacity());

        // Рендерим
        s_vao->bind();
        Renderer::renderVertices(Primitive::POINT, 1);

        // Рендерим детей
        renderChildren();
}

void PictureDepth::initProgram() {
        static bool wasInited = false;
        if (wasInited) return;

        s_program = ProgramPtr(new Program());
        s_program->setDefines({"DEPTH_TEXTURE"});
        s_program->loadSource(pictureProgramSource);
        s_program->build();

        wasInited = true;
}

} //namespace imEngine
