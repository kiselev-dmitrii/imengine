#include <imEngine/Application/GraphicApplication.h>
#include <imEngine/Graphics/Scene/SceneObject.h>
#include <imEngine/Utils/Debug.h>

using namespace imEngine;

class Application : public GraphicApplication {
protected:
        void    initialize();

private:
        SceneObject*      m_world;
        SceneObject*      m_sun;
        SceneObject*      m_earth;
        SceneObject*      m_moon;

};

void Application::initialize() {
        GraphicApplication::initialize();

        m_world = new SceneObject();

        m_sun = new SceneObject(m_world);
        m_sun->setPosition(Vec3(0,0,0));

        m_earth = new SceneObject(m_sun);
        m_earth->setPosition(Vec3(10,10,10));

        m_moon = new SceneObject(m_earth);
        m_moon->setPosition(Vec3(5,5,5));

        IM_VAR(m_sun->worldPosition());
        IM_VAR(m_earth->worldPosition());
        IM_VAR(m_moon->worldPosition());

        m_sun->rotate(Vec3(0,0,1), 90, Space::LOCAL);
        IM_VAR(m_sun->worldPosition());
        IM_VAR(m_earth->worldPosition());
        IM_VAR(m_moon->worldPosition());

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

int main() {
        Application app;
        return app.exec();
}
