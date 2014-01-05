#ifndef GUIMANAGER_H
#define GUIMANAGER_H

#include <imEngine/Graphics/GAPI/GAPI.h>
#include <imEngine/Utils/Types.h>
#include "Widget.h"

namespace imEngine {

class Widget;

struct WidgetGeometry {
        Vec2    size;
        Vec4    texCoords;
};

typedef std::map<String, WidgetGeometry> MapWidgetGeometry;

class GuiManager {
public:
        GuiManager(const String& themeDirectory);

        void            addChild(Widget* wgt)                                  { m_root->addChild(wgt); }
        WidgetGeometry  widgetGeometry(const String& name);
private:
        Widget*                 m_root;
        Texture2DPtr            m_texture;      // текстура со всеми виджетами
        MapWidgetGeometry       m_geometry;     // имя виджета -> соответствующие текст. координаты

};

} //namespace imEngine

#endif // GUIMANAGER_H
