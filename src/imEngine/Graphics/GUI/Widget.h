#ifndef WIDGET_H
#define WIDGET_H

#include <imEngine/Graphics/GAPI/GAPI.h>
#include <imEngine/Utils/TreeNode.h>
#include "GuiManager.h"

namespace imEngine {


/** @brief Базовый класс для виджетов.
 *  Данный класс отвечает за основные свойства виджетов, такие как
 *  их позиция и размер
 */
class Widget : public TreeNode {
public:
        /// Конструктор. paret - указатель на родителя
        explicit Widget(Widget* parent = nullptr);

        /// Инициализация. Выполняется уже после конструктора.
        /// Например можно исполнять метод после присоединения виджета
        virtual void    initialize(GuiManager* manager);

        /// Обновляет состояние виджета
        virtual void    update() = 0;

        /// Рендерит себя и детей
        virtual void    render() = 0;

        /// Устанавливает/возвращает позицию виджета в родительских координатах
        void            setPosition(const Vec2& position);
        Vec2            position() const;

        /// Устанавливает/возвращает позицию виджета в экранных координатах
        void            setAbsolutePosition(const Vec2& position);
        Vec2            absolutePosition();

        /// Возвращает размер виджета
        Vec2            size() const;

        /// Возвращает указатель на менеджера
        GuiManager*     manager() const;

private:
        /// Действия при добавлении, удалении дочерних виджетов
        void            onAttachChild(TreeNode* node);
        void            onDetachChild(TreeNode* node);

        /// Оповещает виджет и дочерние виджеты об изменении позиции
        void            notifyPositionUpdated();
        /// Обновляет абсолютную позицию, если это надо
        void            updateAbsolutePosition();

protected:
        GuiManager*     m_manager;

        Vec2            m_position;
        Vec2            m_absolutePosition;
        bool            m_isNeedToUpdateAbsolutePosition;
        Vec2            m_size;                                         // размер устанавливается в наслудуемых виджетах
};


} //namespace imEngine

#endif // WIDGET_H
