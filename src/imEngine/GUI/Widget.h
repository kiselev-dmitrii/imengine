#ifndef WIDGET_H
#define WIDGET_H

#include <imEngine/Math/Common.h>
#include <imEngine/Resources/Texture.h>

namespace imEngine {

/** @brief Базовый класс для виджетов
 */
class Widget {
public:
        /// Конструктор, parent - ссылка на родителя
        explicit        Widget(Widget* parent = 0);
        /// Виртуальный деструктор
        virtual         ~Widget() = 0;

        /// Размеры виджета
        IVec2           size();
        /// Позиция виджета
        IVec2           positionInParent();
        /// Установить позицию виджета
        void            setPositionInParent(const IVec2& pos);
        /// Позиция виджета в экранных координатах
        IVec2           positionInScreen();
        /// Установить позицию в экранных координатах
        void            setPositionInScreen(const IVec2& pos);

        /// Возвращает указатель на текстуру
        Texture2D       texture();

        /// Возвращает указатель на родительский виджет
        Widget*         parent();
        /// Возвращает i-того потомка данного виджета
        Widget*         children(int i);
        /// Возвращает количество потомков виджета
        uint            countChildren();  
};

} //namespace imEngine

#endif // WIDGET_H
