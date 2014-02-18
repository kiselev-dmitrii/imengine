#include <imEngine/Application/GraphicApplication.h>
#include <imEngine/Graphics/GUI/GuiManager.h>
#include <imEngine/System/Filesystem.h>
#include <imEngine/Utils/Debug.h>
#include <imEngine/Graphics/GUI/TexturedWidget.h>
#include <imEngine/Graphics/GUI/Label.h>
#include <imEngine/Graphics/PrimitiveRenderer.h>

using namespace imEngine;

class Button : public BothStretchableTexturedWidget {
public:
        Button(String active, String hover, String pressed, WidgetAbstract* parent = nullptr) :
                BothStretchableTexturedWidget(active, parent),
                m_isHanded(false),
                m_activeState(active),
                m_hoverState(hover),
                m_pressedState(pressed)
        {  }

        void    update() {
                Mouse*    mouse = manager()->window()->mouse();

                if (glm::all(glm::greaterThanEqual(mouse->position(), IVec2(absolutePosition()))) &&
                    glm::all(glm::lessThanEqual(mouse->position(), IVec2(absolutePosition() + size())))) {
                        if (mouse->isButtonPressed(MouseButton::LEFT)) {
                                setCurrentImage(m_pressedState);
                        } else {
                                setCurrentImage(m_hoverState);
                        }

                        if (mouse->isButtonPressed(MouseButton::RIGHT) && !m_isHanded) {
                                m_isHanded = true;
                                m_delta = Vec2(mouse->position()) - absolutePosition();
                        }


                } else {
                        setCurrentImage(m_activeState);
                }

                if (m_isHanded && mouse->isButtonPressed(MouseButton::RIGHT)) {
                        setAbsolutePosition(Vec2(mouse->position()) - m_delta);
                }
                if (m_isHanded && !mouse->isButtonPressed(MouseButton::RIGHT)) {
                        m_isHanded = false;
                }


        }

private:
        Vec2    m_delta;
        bool    m_isHanded;

        String  m_activeState;
        String  m_hoverState;
        String  m_pressedState;
};


class Application : public GraphicApplication {
protected:
        void    initialize();
        void    update();
        void    render();
        void    destroy();

        void    keyPressEvent(int key);

        void    mouseMoveEvent(int x, int y);

private:
        GuiManager*             m_gui;
        Button*                 m_btn1;
        Button*                 m_btn2;
        Label*                  m_lbl;

};

void Application::initialize() {
        GraphicApplication::initialize();
        glClearColor(1,1,1,1);
        
        m_gui = new GuiManager("resources/gui/elementary/", this);
        m_gui->textureAtlas()->save("gui.png");

        for (auto str: m_gui->imageList()) {
                IM_VAR(str);
                IM_VAR(m_gui->imageGeometry(str)->size);
                IM_VAR(m_gui->imageGeometry(str)->texCoords);
        }

        m_btn1 = new Button("tip_panel.png", "tip_panel.png", "tip_panel.png", m_gui->root());
        m_btn2 = new Button("regular_btn_disabled.png", "regular_btn_focused.png", "regular_btn_active.png", m_btn1);
        m_btn2->setPosition(m_btn1->size());

        m_lbl = new Label("Push", m_btn2);
        m_lbl->setAlignment(LabelAlignment::CENTER);
        m_lbl->setWidth(m_btn2->size().x);
        m_lbl->setPosition(m_lbl->position() + Vec2(0,6));
        m_lbl->setColor(Vec3(0,0,0));

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Application::update() {
        GraphicApplication::update();

        if (window()->keyboard()->isKeyPressed(SDLK_1)) m_btn1->setSize(m_btn1->size() - Vec2(1));
        if (window()->keyboard()->isKeyPressed(SDLK_2)) m_btn1->setSize(m_btn1->size() + Vec2(1));

        if (window()->keyboard()->isKeyPressed(SDLK_3)) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        if (window()->keyboard()->isKeyPressed(SDLK_4)) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Application::render() {
        GraphicApplication::render();
        m_gui->processRender();
}

void Application::destroy() {
        GraphicApplication::destroy();
}

void Application::keyPressEvent(int key) {
        if (key >= 32 && key < 128) {
                char ch = key;
                if (window()->keyboard()->modifiers() & KeyboardModifiers::LSHIFT) ch = toupper(ch);
                m_lbl->setText(m_lbl->text() + ch);
        }
}

void Application::mouseMoveEvent(int x, int y) {
        IM_VAR(Vec2(x,y));
}

int main() {
        Application app;
        app.exec();
}
