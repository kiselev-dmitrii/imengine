#ifndef BOXLAYOUT_H
#define BOXLAYOUT_H

#include "Widget.h"

namespace imEngine {


/** @brief Асбтрактный класс для группировки виджетов.
 *
 * Пример использования:
 * @code
 *      m_lt = new HBoxLayout(parent);
 *      m_btn1 = new Button(...., m_lt);
 *      m_btn2 = new Button(...., m_lt);
 *
 *      m_lt->addWidget(m_btn1);
 *      m_lt->addSpacing(3);
 *      m_lt->addWidget(m_btn2);
 * @endcode
 */
class BoxLayoutAbstract : public WidgetAbstract {
public:
        /// Конструктор
        explicit BoxLayoutAbstract(WidgetAbstract* parent);

        /// Добавляет/вставляет интервал spacing между виджетами
        void            addSpacing(float spacing);
        void            insertSpacing(float spacing, uint position);

        /// Добавляет/вставляет растяжимое пространство.
        /// Чем больше значение factor тем сильнее растягивается область при растяжении виджета
        void            addStretch(float factor);
        void            insertStretch(float factor, uint position);

        /// Меняет местами два элемента на позициях i и j
        void            swap(uint i, uint j);

        /// Рендерит виджеты
        void            onRender() = 0;

protected:
        /// Добавляет/вставляет виджет c выравниванием alignment
        void            addWidgetWithAlignment(WidgetAbstract* widget, int alignment);
        void            insertWidgetWithAlignment(WidgetAbstract* widget, uint position, int alignment);

        /// Обновляет размеры виджета
        virtual void    updateSize() = 0;

protected:
        enum ElementType { WIDGET, SPACING, STRETCH };
        struct Element {
                ElementType     type;
                WidgetAbstract* widget;
                int             alignment;
                float           spacing;
                float           factor;
        };
        typedef std::vector<Element>    ElementList;

protected:
        ElementList     m_elements;
        float           m_factorSum;   //сумма факторов всех stretch-элементов
};


/** @brief Используется для группировки виджетов в горизонтальные строки
 */
class HBoxLayout : public BoxLayoutAbstract, public HStretchableAbstract {
public:
        /// Конструктор
        HBoxLayout(WidgetAbstract* parent);

        /// Добавляет/вставляет виджет c выравниванием alignment
        void    addWidget(WidgetAbstract* widget, WidgetVAlignment::Enum alignment = WidgetVAlignment::TOP);
        void    insertWidget(WidgetAbstract* widget, uint position, WidgetVAlignment::Enum alignment = WidgetVAlignment::TOP);

        /// Устанавливает ширину виджета
        void    setWidth(float width);

        /// Рендерит элементы
        void    onRender();

protected:
        /// Обновляет размер виджета
        void    updateSize();

};

/** @brief Используется для группировки виджетов в вертикальные столбцы
 */
class VBoxLayout : public BoxLayoutAbstract, public VStretchableAbstract {
public:
        /// Конструктор
        VBoxLayout(WidgetAbstract* parent);

        /// Добавляет/вставляет виджет c выравниванием alignment
        void    addWidget(WidgetAbstract* widget, WidgetHAlignment::Enum alignment = WidgetHAlignment::LEFT);
        void    insertWidget(WidgetAbstract* widget, uint position, WidgetHAlignment::Enum alignment = WidgetHAlignment::LEFT);

        /// Устанавливает высоту виджета
        void    setHeight(float height);

        /// Рендерит элементы
        void    onRender();

protected:
        /// Обновляет размер виджета
        void    updateSize();
};

} //namespace imEngine

#endif // BOXLAYOUT_H
