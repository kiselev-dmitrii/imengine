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

        /// Устанавливает выравнивание по умолчанию
        void    setDefaultAlignment(WidgetHAlignment::Enum hAlignment, WidgetVAlignment::Enum vAlignment);

        /// Добавляет виджет
        void    addWidget(WidgetAbstract* widget, WidgetHAlignment::Enum horizontalAlignment, WidgetVAlignment::Enum verticalAlignment);
        void    addWidget(WidgetAbstract* widget);

        /// Сворачивает виджет виджет
        void    foldWidget();

        /// Добавляет пустое пространство
        void    addSpacing(float space);


        /// Вставляет виджет в позицию position
        void    insertWidget(WidgetAbstract* widget, uint position);
        /// Вставляет пустое пространство в позицию position
        void    insertSpacing(float space, uint position);

private:
        struct Element {
                WidgetAbstract*         widget;
                WidgetHAlignment::Enum  hAlignment;
                WidgetVAlignment::Enum  vAlignment;
                float                   width;
        };

        std::vector<Element>            m_elements;
};

} //namespace imEngine

#endif // BOXLAYOUT_H
