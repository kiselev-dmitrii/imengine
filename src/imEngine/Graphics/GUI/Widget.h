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
class WidgetAbstract : public TreeNode {
public:
        /// Конструктор. paret - указатель на родителя
        explicit WidgetAbstract(WidgetAbstract* parent);

        /// Рендерит себя и детей
        virtual void    render() = 0;

       /// Вызывается при нахождении мыши над виджетом.
        virtual bool    onMouseMove(int x, int y)                               { return false; }
        /// Возникает, когда виджет входит в пространство над виджетом
        virtual bool    onMouseEnter(int x, int y)                              { return false; }
        /// Возникает, когда виджет покидает пространство над виджетом
        virtual bool    onMouseLeave(int x, int y)                              { return false; }
        /// Возникает, когда на виджете нажали
        virtual bool    onMousePress(int x, int y, char button)                 { return false; }
        /// Возникает, когда на виджете отпустили кнопку
        virtual bool    onMouseRelease(int x, int y, char button)               { return false; }

        /// Возникает, когда мышь движется где угодно
        virtual void    onGlobalMouseMove(int x, int y)                         { }
        /// Возникает, когда клавиша мыши нажимаются где угодно
        virtual void    onGlobalMousePress(int x, int y, char button)           { }
        /// Возникает, когда клавиша мыши отжимается где угодно
        virtual void    onGlobalMouseRelease(int x, int y, char button)         { }

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

public:
        /// Обработка движений мыши
        bool            processMouseMove(int oldX, int oldY, int newX, int newY);
        /// Обработка нажатий клавиш мыши
        bool            processMousePress(int x, int y, char button);
        /// Обработка отжатий клавиш мыши
        bool            processMouseRelease(int x, int y, char button);
        /// Обработка движений мыши везде
        void            processGlobalMouseMove(int x, int y);
        /// Обработка нажатий мыши везде
        void            processGlobalMousePress(int x, int y, char button);
        /// Обработка отжатий мыши везде
        void            processGlobalMouseRelease(int x, int y, char button);

protected:
        /// Рендерит детишек данного виджета
        void            renderChildren();

        /// Определяет, находится ли точка x,y внутри виджета
        bool            isInsideWidget(int x, int y);

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


/** @brief Абстрактный класс, для виджетов, которые могут растягиваться
 *  по горизонтали
 */
class HStretchableAbstract {
public:
        /// Конструктор
        HStretchableAbstract()                                                  { m_minimalWidth = 0; }

        /// Установка ширины
        virtual void    setWidth(float width) = 0;

        /// Установка минимальной ширины
        void            setMinimalWidth(float width)                            { m_minimalWidth = width; }
        /// Получение минимальной ширины
        float           minimalWidth() const                                    { return m_minimalWidth; }

private:
        float           m_minimalWidth;
};


/** @brief Абстрактный класс, для виджетов, которые могут растягиваться только
 *  по вертикали
 */
class VStretchableAbstract {
public:
        /// Конструктор
        VStretchableAbstract()                                                  { m_minimalHeight = 0; }

        /// Установка высоты
        virtual void    setHeight(float height) = 0;

        /// Установка минимальной высоты
        void            setMinimalHeight(float height)                          { m_minimalHeight = height; }
        /// Получения минимальной высоты
        float           minimalHeight() const                                   { return m_minimalHeight; }

private:
        float           m_minimalHeight;
};


/** @brief Абстрактный класс, для виджетов, которые могут растягиваться и
 *  по вертикали и по горизонтали
 */
class BothStretchableAbstract {
public:
        /// Конструктор
        BothStretchableAbstract()                                               { m_minimalSize = Vec2(0,0); }

        /// Установка высоты
        virtual void    setWidth(float width) = 0;
        /// Установка ширины
        virtual void    setHeight(float height) = 0;
        /// Установка размеров
        void            setSize(const Vec2& size)                               { setWidth(size.x); setHeight(size.y); }

        /// Установка минимальных размеров
        void            setMinimalSize(const Vec2& size)                        { m_minimalSize = size; }
        /// Получение минимальных размеров
        Vec2            minimalSize() const                                     { return m_minimalSize; }

private:
        Vec2            m_minimalSize;
};

} //namespace imEngine

#endif // WIDGET_H
