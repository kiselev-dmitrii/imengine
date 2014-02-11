#include <imEngine/Application/BaseApplication.h>
#include <imEngine/Graphics/GAPI/Texture/Texture3D.h>
#include <sstream>
#include <iomanip>
#include <imEngine/Utils/Debug.h>

using namespace imEngine;

class Application : public BaseApplication {
public:
        void    initialize();
        void    update();
        void    render();
        void    destroy();

private:
        Texture3D*      m_engine;
        Texture3D*      m_head;
};

void Application::initialize() {
        m_engine = new Texture3D();
        m_engine->load(256,256,128, InternalFormat::COLOR_NORM_1_COMP_8_BIT, SourceType::UBYTE, SourceFormat::R, "resources/texture/engine.raw");
        IM_VAR(m_engine->width());
        IM_VAR(m_engine->height());
        IM_VAR(m_engine->depth());
        IM_VAR(m_engine->wasMemoryAllocated());
        IM_VAR(m_engine->sizeOfData());

        m_head = new Texture3D();
        StringList filenames;
        /// vhm.1001.dcm - vhm.1245.dcm
        for (uint i = 1; i <= 245; ++i) {
                std::stringstream ss;
                ss << "vhm.1" << std::setw(3) << std::setfill('0') << i << ".dcm";
                filenames.push_back("resources/texture/head/" + ss.str());
        }
        m_head->load(filenames);
        IM_VAR(m_head->width());
        IM_VAR(m_head->height());
        IM_VAR(m_head->depth());
        IM_VAR(m_head->wasMemoryAllocated());
        IM_VAR(m_head->sizeOfData());

}

void Application::update() {

}

void Application::render() {

}

void Application::destroy() {
        delete m_engine;
        delete m_head;
}

int main() {
        Application app;
        return app.exec();
}
