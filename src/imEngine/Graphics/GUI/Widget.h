#ifndef WIDGET_H
#define WIDGET_H

#include <imEngine/Graphics/GAPI/GAPI.h>
#include <imEngine/Utils/TreeNode.h>
#include "GuiManager.h"

namespace imEngine {


/** @brief Базовый класс для виджетов
 */
class Widget : public TreeNode {
public:
        /// Конструктор, parent - ссылка на родителя
        explicit                Widget()  {}

        void                    setPosition(const Vec2& position) {}
        Vec2                    position() const {return Vec2(0,0); }
        virtual void            render() = 0;

protected:
        void                    renderChilds() {}

private:
        GuiManager*             m_manager;
        ImageGeometry           m_geometry;
        Vec2                    m_position;
};


} //namespace imEngine

#endif // WIDGET_H
