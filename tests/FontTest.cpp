#include <imEngine/Application/BaseApplication.h>
#include <imEngine/Graphics/GAPI/GAPI.h>
#include <imEngine/FileContainers/Xml.h>
#include <imEngine/Utils/StringUtils.h>
#include <imEngine/Utils/TreeNode.h>
#include <imEngine/Utils/Debug.h>
#include <imEngine/FileContainers/Font.h>
#include <cstddef>
#include "showTexture.glsl"

using namespace imEngine;

/*
class AtlasElement {
public:
        AtlasElement(IVec2 startCoords, IVec2 endCoords, uint numFrames);

        uint    numberOfFrames()                                                { return m_numFrames; }
        IVec2   sizeOfFrame();
        IVec2   startCoordsOfFrame(uint frame = 0);
        IVec2   endCoordsOfFrame(uint frame = 0);

private:
        IVec2   m_startCoords;
        IVec2   m_endCoords;
        uint    m_numFrames;
};

AtlasElement::AtlasElement(IVec2 startCoords, IVec2 endCoords, uint numFrames) :
        m_startCoords(startCoords),
        m_endCoords(endCoords),
        m_numFrames(numFrames)
{ }

IVec2 AtlasElement::sizeOfFrame() {
        IVec2 size = ((m_endCoords-m_startCoords) + IVec2(1,1));
        size.x /= m_numFrames;
        return size;
}

IVec2 AtlasElement::startCoordsOfFrame(uint frame) {
        return IVec2(m_startCoords.x + sizeOfFrame().x * frame, m_startCoords.y);
}

IVec2 AtlasElement::endCoordsOfFrame(uint frame) {
        return IVec2(m_startCoords.x + sizeOfFrame().x * (frame+1) - 1, m_endCoords.y);
}


class GuiTheme {
public:
        GuiTheme(const XmlDocument& document);

        const String&           textureName()                                   { return m_textureName; }

        AtlasElement&           button()                                        { return m_button; }
        AtlasElement&           linebox()                                       { return m_linebox; }

private:
        AtlasElement    parseButton(const XmlNode& node);
        AtlasElement    parseLinebox(const XmlNode& node);

private:
        String          m_textureName;
        AtlasElement    m_button;
        AtlasElement    m_linebox;
};

GuiTheme::GuiTheme(const XmlDocument &document) :
        m_textureName(document.child("theme").attribute("texture").value()),
        m_button(parseButton(document.child("theme").child("button"))),
        m_linebox(parseButton(document.child("theme").child("linebox")))
{
}

AtlasElement GuiTheme::parseButton(const XmlNode &node) {
        StringList tmp = StringUtils::split(node.attribute("start").value(), ",");
        IVec2 start = IVec2(std::stoi(tmp[0]), std::stoi(tmp[1]));

        tmp = StringUtils::split(node.attribute("end").value(), ",");
        IVec2 end = IVec2(std::stoi(tmp[0]), std::stoi(tmp[1]));

        uint numFrames = std::stoi(node.attribute("frames").value());

        return AtlasElement(start, end, numFrames);
}

AtlasElement GuiTheme::parseLinebox(const XmlNode &node) {
        StringList tmp = StringUtils::split(node.attribute("start").value(), ",");
        IVec2 start = IVec2(std::stoi(tmp[0]), std::stoi(tmp[1]));

        tmp = StringUtils::split(node.attribute("end").value(), ",");
        IVec2 end = IVec2(std::stoi(tmp[0]), std::stoi(tmp[1]));

        uint numFrames = std::stoi(node.attribute("frames").value());

        return AtlasElement(start, end, numFrames);
}

class GuiManager {
        friend class Widget;
public:
        GuiManager(const String& themeXmlFile);
        ~GuiManager();

        void            render();

private:
        Texture2D*      m_texture;
        VertexBuffer*   m_vbo;
        Program*        m_program;
};


class Widget : public TreeNode {
public:
        /// Конструктор, parent - ссылка на родителя
        explicit                Widget(Widget* parent);
        /// Конструктор, manager - ссылка на GuiManager
        explicit                Widget(GuiManager* manager);

        /// Позиция виджета
        IVec2                   positionInParent();
        /// Установить позицию виджета
        void                    setPositionInParent(const IVec2& pos);
        /// Позиция виджета в экранных координатах
        IVec2                   positionInScreen();
        /// Установить позицию в экранных координатах
        void                    setPositionInScreen(const IVec2& pos);

        /// Размеры виджета
        IVec2                   size();

        /// Возвращает указатель на текстуру
        Texture2D*              texture();

private:
        void                    updateVBO();

};
*/

struct AABBSprite {
        Vec3    topLeftPos;
        Vec2    size;
        Vec4    texCoords;
};
typedef std::vector<AABBSprite> AABBSpriteList;

AABBSpriteList generateFontSprites(const String& text, const Vec2& pos, const Font& font) {
        AABBSpriteList sprites;
        sprites.reserve(text.size());

        Vec2 prevPos = pos;
        for(char ch: text) {
                Glyph g = font.glyph(ch);

                AABBSprite sprite;
                sprite.size = g.size;
                sprite.texCoords = g.texCoords;
                sprite.topLeftPos = Vec3(prevPos + Vec2(g.bearing.x, -g.bearing.y), 0.0) ;

                prevPos += Vec2(g.advance.x, -g.advance.y);

                sprites.push_back(sprite);
        }
        return sprites;
}

class Application : public BaseApplication {
protected:
        void    initialize();
        void    update();
        void    render();
        void    destroy();

        void    windowResizeEvent(int x, int y);
        void    keyPressEvent(int key);

private:
        Texture2DPtr    m_texture;
        Program*        m_program;
        VertexBuffer*   m_vbo;
        VertexArray*    m_vao;
};

void Application::initialize() {
        glClearColor(0,0,0,1);
        Font font("resources/font/Lobster.ttf", 48);

        // Создаю программку
        m_program = new Program();
        m_program->loadSource(showTextureSource);
        m_program->build();

        // Текстура со ширфтом
        m_texture = font.texture();
        m_texture->setMagnificationFilter(TextureMagFilter::LINEAR);
        m_texture->setMinimizationFilter(TextureMinFilter::LINEAR);

        // Генерирую VBO для надпись
        AABBSpriteList sprites = generateFontSprites("imEngine", Vec2(320, 240), font);
        for (AABBSprite& sprite: sprites) {
                IM_VAR(sprite.topLeftPos);
                IM_VAR(sprite.size);
                IM_VAR(sprite.texCoords);
        }

        m_vbo = new VertexBuffer();
        m_vbo->load(&sprites[0], sizeof(AABBSprite)*sprites.size(), BufferUsage::STREAM_DRAW);
        m_vao = new VertexArray();
        m_vao->bind();
                m_vbo->connect(m_program->attributeLocation("in_position"), 3, GL_FLOAT, offsetof(AABBSprite, topLeftPos), sizeof(AABBSprite));
                m_vbo->connect(m_program->attributeLocation("in_size"), 2, GL_FLOAT, offsetof(AABBSprite, size) , sizeof(AABBSprite));
                m_vbo->connect(m_program->attributeLocation("in_texCoords"), 4, GL_FLOAT, offsetof(AABBSprite, texCoords), sizeof(AABBSprite));
        m_vao->unbind();

        m_program->bind();
        m_texture->bind(0);
        m_program->setUniform("fontAtlas", 0);
        m_program->setUniform("winSize", Vec2(mainWindow()->size()));
        IM_VAR(Vec2(mainWindow()->size()));


        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Application::update() {
}

void Application::render() {
        Renderer::clearBuffers();
        m_vao->bind();
        Renderer::renderVertices(Primitive::POINT, 8);
}

void Application::windowResizeEvent(int x, int y) {
}

void Application::keyPressEvent(int key) {

}

void Application::destroy() {
}

int main() {
        Application app;
        app.exec();
}
