#include "BoxLayout.h"

namespace imEngine {

//######################### BoxLayoutAbstract ################################//

BoxLayoutAbstract::BoxLayoutAbstract(WidgetAbstract *parent) :
        WidgetAbstract(parent),
        m_factorSum(0)
{
        m_size = Vec2(0,0);
}

void BoxLayoutAbstract::addSpacing(float spacing) {
        insertSpacing(spacing, m_elements.size());
}

void BoxLayoutAbstract::insertSpacing(float spacing, uint position) {
        Element spc;
        spc.type = SPACING;
        spc.spacing = spacing;
        m_elements.insert(m_elements.begin() + position, spc);

        updateSize();
}

void BoxLayoutAbstract::addStretch(float factor) {
        insertStretch(factor, m_elements.size());
}

void BoxLayoutAbstract::insertStretch(float factor, uint position) {
        Element str;
        str.type = STRETCH;
        str.factor = factor;
        m_elements.insert(m_elements.begin() + position, str);

        // Stretch не влияет на размер виджета, но нам необходима сумма всех факторов
        // чтобы правильно расчитать размер отступаемого пространства при рендеринге
        m_factorSum += factor;
}

void BoxLayoutAbstract::swap(uint i, uint j) {
        std::swap(m_elements[i], m_elements[j]);
}

void BoxLayoutAbstract::addWidgetWithAlignment(WidgetAbstract *widget, int alignment) {
        insertWidgetWithAlignment(widget, m_elements.size(), alignment);
}

void BoxLayoutAbstract::insertWidgetWithAlignment(WidgetAbstract *widget, uint position, int alignment) {
        Element wgt;
        wgt.type = WIDGET;
        wgt.widget = widget;
        wgt.alignment = alignment;
        m_elements.insert(m_elements.begin() + position, wgt);

        updateSize();
}


//############################## HBoxLayout ##################################//

HBoxLayout::HBoxLayout(WidgetAbstract *parent) :
        BoxLayoutAbstract(parent),
        HStretchableAbstract()
{
        setMinimalWidth(0);
}

void HBoxLayout::addWidget(WidgetAbstract *widget, WidgetVAlignment::Enum alignment) {
        addWidgetWithAlignment(widget, alignment);
}

void HBoxLayout::insertWidget(WidgetAbstract *widget, uint position, WidgetVAlignment::Enum alignment) {
        insertWidgetWithAlignment(widget, position, alignment);
}

void HBoxLayout::setWidth(float width) {
        if (width < minimalWidth()) return;

        m_size.x = width;
        onWidthChange();
}

void HBoxLayout::onRender() {
        // Ширина, которая приходится на все STRETCH
        float totalStretchWidth = width() - minimalWidth();

        // Выставляем координаты виджетов и рендерим
        float x = 0;
        for (Element& element: m_elements) {
                switch (element.type) {
                        case WIDGET:
                                if (element.widget->isVisible()) {
                                        element.widget->setLeft(x);
                                        element.widget->alignVertical((WidgetVAlignment::Enum)element.alignment);
                                        element.widget->processRender();

                                        x += element.widget->width();
                                }
                                break;

                        case SPACING:
                                x += element.spacing;
                                break;

                        case STRETCH:
                                x += (element.factor/m_factorSum) * totalStretchWidth;
                                break;
                }
        }
}


void HBoxLayout::updateSize() {
        float width = 0;
        float height = 0;

        for (Element& element: m_elements) {
                if (element.type == WIDGET) {
                        width += element.widget->width();
                        height = std::max(height, element.widget->height());
                } else if (element.type == SPACING) {
                        width += element.spacing;
                }
        }

        // Обновляем минимальные размеры
        setMinimalWidth(width);

        // Обновляем размеры
        if (m_size.x < width) m_size.x = width;
        m_size.y = height;
}

void HBoxLayout::loadChildrenFromXml(const XmlNode &node) {
        for (XmlNode& child: node.children()) {
                /// Обрабатываем сначала элементы не являющиеся виджетами
                /// а затем добавлям виджеты
                String type = child.name();
                if (type == "spacing") {
                        String spacing = child.attribute("spacing").value();
                        addSpacing(std::stof(spacing));

                } else if(type == "stretch") {
                        String factor = child.attribute("factor").value();
                        addStretch(std::stof(factor));

                } else {
                        WidgetAbstract* widget = createWidget(child);

                        WidgetVAlignment::Enum vAlignment =  WidgetVAlignment::TOP;
                        String alignment = child.attribute("alignment").value();
                        if (alignment == "CENTER") vAlignment = WidgetVAlignment::CENTER;
                        else if (alignment == "BOTTOM") vAlignment = WidgetVAlignment::BOTTOM;
                        else vAlignment = WidgetVAlignment::TOP;
                        addWidget(widget, vAlignment);

                        widget->loadFromXml(child);
                }
        }
}

//############################## HBoxLayout ##################################//

VBoxLayout::VBoxLayout(WidgetAbstract *parent) :
        BoxLayoutAbstract(parent),
        VStretchableAbstract()
{
        setMinimalHeight(0);
}

void VBoxLayout::addWidget(WidgetAbstract *widget, WidgetHAlignment::Enum alignment) {
        addWidgetWithAlignment(widget, alignment);
}

void VBoxLayout::insertWidget(WidgetAbstract *widget, uint position, WidgetHAlignment::Enum alignment) {
        insertWidgetWithAlignment(widget, position, alignment);
}

void VBoxLayout::setHeight(float height) {
        if (height < minimalHeight()) return;

        m_size.y = height;
        onHeightChange();
}

void VBoxLayout::onRender() {
        // Высота, приходящаяся на все STRETCH
        float totalStretchHeight = height() - minimalHeight();

        // Рендерим виджеты
        float y = 0;
        for (Element& element: m_elements) {
                switch (element.type) {
                        case WIDGET:
                                if (element.widget->isVisible()) {
                                        element.widget->setTop(y);
                                        element.widget->alignHorizontal((WidgetHAlignment::Enum)element.alignment);
                                        element.widget->processRender();

                                        y += element.widget->height();
                                }
                                break;

                        case SPACING:
                                y += element.spacing;
                                break;

                        case STRETCH:
                                y += (element.factor/m_factorSum) * totalStretchHeight;
                                break;
                }
        }
}

void VBoxLayout::updateSize() {
        float width = 0;
        float height = 0;

        for (Element& element: m_elements) {
                if (element.type == WIDGET) {
                        width = std::max(width, element.widget->width());
                        height += element.widget->height();
                } else if (element.type == SPACING) {
                        height += element.spacing;
                }
        }

        // Обновляем минимальные размеры
        setMinimalHeight(height);

        // Обновляем размеры
        m_size.x = width;
        if (m_size.y < height) m_size.y = height;
}

void VBoxLayout::loadChildrenFromXml(const XmlNode &node) {
        for (XmlNode& child: node.children()) {
                /// Обрабатываем сначала элементы не являющиеся виджетами
                /// а затем добавлям виджеты
                String type = child.name();
                if (type == "spacing") {
                        String spacing = child.attribute("spacing").value();
                        addSpacing(std::stof(spacing));

                } else if(type == "stretch") {
                        String factor = child.attribute("factor").value();
                        addStretch(std::stof(factor));

                } else {
                        WidgetAbstract* widget = createWidget(child);

                        WidgetHAlignment::Enum hAlignment =  WidgetHAlignment::LEFT;
                        String alignment = child.attribute("alignment").value();
                        if (alignment == "CENTER") hAlignment = WidgetHAlignment::CENTER;
                        else if (alignment == "RIGHT") hAlignment = WidgetHAlignment::RIGHT;
                        else hAlignment = WidgetHAlignment::LEFT;
                        addWidget(widget, hAlignment);

                        widget->loadFromXml(child);
                }
        }
}

} //namespace imEngine
