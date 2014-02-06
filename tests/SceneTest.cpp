#include <imEngine/Application/GraphicApplication.h>
#include <imEngine/Graphics/Scene/Scene.h>
#include <imEngine/Utils/Debug.h>
#include <imEngine/Graphics/Scene/Entity/PolygonalEntity.h>

using namespace imEngine;

class Application : public GraphicApplication {
protected:
        void    initialize();

private:
        PolygonalEntity*        m_box1;
        PolygonalEntity*        m_box2;
};


void Application::initialize() {
        GraphicApplication::initialize();

        Renderer::setBlendMode(BlendMode::ALPHA);
        m_box1 = new PolygonalEntity(scene()->world());
        m_box2 = new PolygonalEntity(scene()->world());
        m_box2->setPosition(Vec3(2,2,2));
}

int main() {
        Application app;
        return app.exec();
}
