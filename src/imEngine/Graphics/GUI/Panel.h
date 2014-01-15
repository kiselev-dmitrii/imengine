#ifndef PANEL_H
#define PANEL_H

#include "TexturedWidget.h"

namespace imEngine {


/** Класс, представляющий собой виджет, который может перемещаться и
 *  содержать другие виджеты
 */
class Panel : public BothStretchableTexturedWidget {
public:
        /// Конструктор. На вход получает изображение с панелью
        Panel(const String& image, WidgetAbstract* parent);

        /// Обработка перетаскивания панели
        bool    onMousePress(int x, int y, char button);
        bool    onMouseRelease(int x, int y, char button);
        bool    onMouseMove(int x, int y);

private:
        bool    m_wasMousePressed;                                              // true - мышь была нажата, fasle - отжата
        Vec2    m_mouseOffset;                                                  // смещение мыши от верхнего левого угла
};


} //namespace imEngine

#endif // PANEL_H
