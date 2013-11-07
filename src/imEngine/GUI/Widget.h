#ifndef WIDGET_H
#define WIDGET_H

namespace imEngine {

/** @brief Базовый класс для виджетов
 */
class Widget {
public:
        explicit        Widget(Widget* parent);

        int             width();
        int             height();
};

} //namespace imEngine

#endif // WIDGET_H
