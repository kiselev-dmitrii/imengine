#ifndef WIDGET_H
#define WIDGET_H

#include <imEngine/Math/Common.h>
#include <imEngine/Graphics/GAPI/GAPI.h>
#include <imEngine/Utils/TreeNode.h>

namespace imEngine {

/** @brief Базовый класс для виджетов
 */
class Widget : public TreeNode {
public:
        /// Конструктор, parent - ссылка на родителя
        explicit                Widget(Widget* parent = 0);

        /// Позиция виджета
        IVec2                   positionInParent();
        /// Установить позицию виджета
        void                    setPositionInParent(const IVec2& pos);

        /// Позиция виджета в экранных координатах
        IVec2                   positionInScreen();
        /// Установить позицию в экранных координатах
        void                    setPositionInScreen(const IVec2& pos);

        /// Размеры виджета
        IVec2                   size();

        /// Возвращает указатель на текстуру
        Texture2D*              texture();


};

} //namespace imEngine

#endif // WIDGET_H
