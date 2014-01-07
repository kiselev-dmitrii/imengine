#include <imEngine/Application/BaseApplication.h>
#include <imEngine/Graphics/GUI/GuiManager.h>
#include <imEngine/System/Filesystem.h>
#include <imEngine/Utils/Debug.h>
#include <imEngine/Graphics/GUI/DrawableWidget.h>

using namespace imEngine;

class Button : public HStretchableWidget {
public:
        Button(String active, String hover, String pressed, Widget* parent = nullptr) :
                HStretchableWidget(active, parent),
                m_activeState(active),
                m_hoverState(hover),
                m_pressedState(pressed),
                m_isHanded(false)
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


                for (TreeNode* node: children()) ((Widget*)node)->update();
        }

private:
        Vec2    m_delta;
        bool    m_isHanded;

        String  m_activeState;
        String  m_hoverState;
        String  m_pressedState;
};


class Application : public BaseApplication {
protected:
        void    initialize();
        void    update();
        void    render();
        void    destroy();

private:
        GuiManager*             m_gui;
        Button*                 m_btn;
        Button*                 m_btn2;

};

void Application::initialize() {
        glClearColor(0,0,0,0);
        
        m_gui = new GuiManager("resources/gui/elementary/", mainWindow());
        m_gui->textureAtlas()->save("gui.png");

        for (auto str: m_gui->imageList()) {
                IM_VAR(str);
                IM_VAR(m_gui->imageGeometry(str)->size);
                IM_VAR(m_gui->imageGeometry(str)->texCoords);
        }

        m_btn = new Button("regular_btn_active.png", "regular_btn_hover.png", "regular_btn_pressed.png");
        m_gui->attachWidget(m_btn);
        m_btn2 = new Button("regular_btn_disabled.png", "regular_btn_focused.png", "regular_btn_active.png");
        m_btn2->setPosition(m_btn->size());
        m_btn->attachChild(m_btn2);
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void Application::update() {
        m_gui->update();

        if (mainWindow()->keyboard()->isKeyPressed(SDLK_1)) m_btn2->setWidth(m_btn2->size().x - 1);
        if (mainWindow()->keyboard()->isKeyPressed(SDLK_2)) m_btn2->setWidth(m_btn2->size().x + 1);
}

void Application::render() {
        Renderer::clearBuffers();
        m_gui->render();
}

void Application::destroy() {
}

int main() {
        Application app;
        app.exec();
}
