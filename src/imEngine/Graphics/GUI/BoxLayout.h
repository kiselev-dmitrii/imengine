#ifndef BOXLAYOUT_H
#define BOXLAYOUT_H

#include "Widget.h"

namespace imEngine {


/** @brief Используется для группировки виджетов в горизонтальные строки
 * Добавление виджетов следует осуществлять через метод addWidget,
 * так как при этом можно задать дополнительную информацию.
 * При этом виджеты также должны быть сыновьями HBoxLayout
 */
class HBoxLayout : public WidgetAbstract, public HStretchableAbstract {
public:
        /// Конструктор
        HBoxLayout(WidgetAbstract* parent);

        /// Устанавливает ширину виджета
        void    setWidth(float width);

        /// Добавляет/вставляет виджет
        void    addWidget(WidgetAbstract* widget, WidgetVAlignment::Enum vAlignment = WidgetVAlignment::TOP);
        void    insertWidget(WidgetAbstract *widget, uint position, WidgetVAlignment::Enum vAlignment = WidgetVAlignment::TOP);

        /// Добавляет/вставляет пространство размера space
        void    addSpacing(float space);
        void    insertSpacing(float space, uint position);

        /// Добавляет пустое растяжимое пространство
        void    addSpring(float factor);

        /// Меняет местами два элемента
        void    swapElements(uint i, uint j);

public:
        /// Рендерит элементы
        void    onRender();

private:
        enum ElementType {
                WIDGET,
                SPACING,
                SPRING
        };
        struct Element {
                ElementType             type;
                WidgetAbstract*         widget;
                WidgetVAlignment::Enum  vAlignment;
                float                   width;
                float                   factor;
        };

        std::vector<Element>            m_elements;
};

} //namespace imEngine

#endif // BOXLAYOUT_H
