#ifndef WIDGET_H
#define WIDGET_H

namespace imFramework {

/** @brief Базовый класс для виджетов
 */
class Widget {
public:
        explicit        Widget(Widget* parent);

        int             width();
        int             height();
};

} //namespace imFramework

#endif // WIDGET_H
