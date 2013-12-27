#ifndef WIDGET_H
#define WIDGET_H

#include <imEngine/Math/Common.h>

namespace imEngine {

/** @brief Базовый класс для виджетов
 */
class Widget {
public:
        explicit        Widget(Widget* parent);

        virtual Vec2    size();
        virtual Vec2    position();
};

} //namespace imEngine

#endif // WIDGET_H
