#include "Label.h"
#include <imEngine/Utils/Debug.h>
#include <imEngine/Graphics/PrimitiveRenderer.h>

namespace imEngine {

Label::Label(const String &text, WidgetAbstract *parent) :
        WidgetAbstract(parent),
        HStretchableAbstract(),
        m_alignment(LabelAlignment::LEFT),
        m_tmpString(text)
{ }

void Label::initialize(GuiManager *manager) {
        WidgetAbstract::initialize(manager);
        m_text = TextPtr(new Text(m_tmpString, Font::defaultFont(), manager->window()));
        m_size = Vec2(m_text->font()->calcSizeOfText(m_text->text()));
        setMinimalWidth(0);
}

void Label::update() {  }

void Label::render() {
        Vec2 wgtPos = absolutePosition();
        Vec2 wgtSize = size();
        float txtWidth = m_text->size().x;

        // Вычисляем позицию текста
        Vec2 txtPos;
        switch (m_alignment) {
                case LabelAlignment::LEFT:
                        txtPos.x = wgtPos.x;
                        break;
                case LabelAlignment::CENTER:
                        txtPos.x = wgtPos.x + wgtSize.x/2 - txtWidth/2;
                        break;
                case LabelAlignment::RIGHT:
                        txtPos.x = wgtPos.x + wgtSize.x - txtWidth;
                        break;
        }
        txtPos.y = wgtPos.y + wgtSize.y/2;
        m_text->setPosition(txtPos);

        Renderer::beginStencilRendering();
                PrimitiveRenderer::instance().drawRectangleInScreenSpace(wgtPos, wgtSize, Vec3(1,0,0), manager()->window());
        Renderer::continueStencilRendering();
                m_text->render();
        Renderer::endStencilRendering();

        renderChildren();
}

} //namespace imEngine
