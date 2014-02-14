#ifndef WIDGET_H
#define WIDGET_H

#include <imEngine/Graphics/GAPI/GAPI.h>
#include <imEngine/Utils/TreeNamedNode.h>
#include "GuiManager.h"


namespace imEngine {

/** @brief Выравнивание виджета по горизонтали
 */
namespace WidgetHAlignment {
enum Enum {
        LEFT,
        CENTER,
        RIGHT
};
}

/** @brief Выравнивание виджета по вертикали
 */
namespace WidgetVAlignment {
enum Enum {
        TOP,
        CENTER,
        BOTTOM
};
}

/** @brief Структура, задающая внутренние отступы для виджета
 */
struct WidgetPadding {
        float   left;
        float   top;
        float   right;
        float   bottom;
};


/** @brief Базовый класс для виджетов.
 *  Данный класс отвечает за основные свойства виджетов, такие как
 *  их позиция и размер
 */
class WidgetAbstract : public TreeNamedNode<WidgetAbstract> {
public:
        /// Конструктор. paret - указатель на родителя
        explicit WidgetAbstract(WidgetAbstract* parent);

        /// Устанавливает/возвращает видимость виджета
        void            setVisible(bool isVisible);
        bool            isVisible() const;
        /// Скрывает/показывает виджет
        void            hide();
        void            show();

        /// Устанавливает/возвращает включенность виджета
        void            setEnabled(bool isEnabled);
        bool            isEnabled() const;
        /// Отключает/включает виджет от пользовательского ввода
        void            disable();
        void            enable();

        /// Устанавливает/убирает фокус
        void            setFocus();
        void            clearFocus();

        /// Устанавливает/возвращает уровень непрозрачности для виджета и детей
        /// 0 - прозрачный, 1 - непрозрачный
        void            setOpacity(float opacity);
        float           opacity() const;

        /// Устанавливает/возвращает позицию виджета в родительских координатах
        void            setLeft(float x);
        void            setTop(float y);
        void            setPosition(const Vec2& position);
        float           left() const;
        float           top() const;
        Vec2            position() const;

        /// Устанавливает/возвращает внутрений отступ
        void            setPadding(const WidgetPadding& padding);
        void            setPadding(float left, float top, float right, float bottom);
        void            setPadding(float offset);
        WidgetPadding   padding() const;

        /// Выравнивает виджет относительно родителя
        void            alignHorizontal(WidgetHAlignment::Enum align);
        void            alignVertical(WidgetVAlignment::Enum align);

        /// Устанавливает/возвращает позицию виджета в экранных координатах
        void            setAbsolutePosition(const Vec2& position);
        Vec2            absolutePosition();

        /// Конвертирует вектор из абсолютной СК в локальную СК
        Vec2            absoluteToLocal(const Vec2& pos);
        /// Конвертирует из локальной СК в абсолютную СК
        Vec2            localToAbsolute(const Vec2& pos);

        /// Возвращает размер виджета
        Vec2            size() const;
        float           width() const;
        float           height() const;

        /// Возвращает внутренние размеры виджета
        Vec2            contentSize() const;
        float           contentWidth() const;
        float           contentHeight() const;

        /// Возвращает указатель на менеджера
        GuiManager*     manager() const;

public:
        /// Возвращает текущий виджет в фокусе
        static WidgetAbstract*  focusedWidget();

public:
        /// Вызывается при визуализации виджета.
        virtual void    onRender()                                              { renderChildren(); }
        /// Вызывается каждый кадр перед рендером.
        virtual void    onUpdate()                                              { }

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

        /// Возникает, когда клавиша клавиатуры нажата (при установленном фокусе)
        virtual void    onKeyPress(int key)                                     { }
        /// Возникает, когда клавиша клавиатуры отжата (при установленном фокусе)
        virtual void    onKeyRelease(int key)                                   { }

        /// Вызывается, при отключении виджета от ввода
        virtual void    onWidgetDisable()                                       { }
        /// Вызывается, при включении виджета
        virtual void    onWidgetEnable()                                        { }

        /// Вызывается, когда виджет получает фокус
        virtual void    onWidgetSetFocus()                                      { }
        /// Вызывается, когда виджет теряет фокус
        virtual void    onWidgetClearFocus()                                    { }

public:
        /// Рендер виджета и его детей
        void            processRender();
        /// Обновление виджета и его детей
        void            processUpdate();
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
        /// Определяет, находится ли точка x,y внутри виджета
        bool            isInsideWidget(int x, int y);
        /// Рендерит детей виджета
        void            renderChildren();

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
        bool            m_isVisible;
        bool            m_isEnabled;
        float           m_opacity;                                      // уровень непрозрачности (от 0 - прозрачный до 1 - непрозрачный)

        WidgetPadding   m_padding;                                      // внутренние отступы

private:
        static WidgetAbstract*  s_focusedWidget;

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

        /// Вызывается при изменении ширины
        virtual void    onWidthChange()                                         { }

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

        /// Вызывается при изменении высоты
        virtual void    onHeightChange()                                        { }

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

        /// Вызывается при изменении ширины
        virtual void    onWidthChange()                                         { }
        /// Вызывается при изменении высоты
        virtual void    onHeightChange()                                        { }

private:
        Vec2            m_minimalSize;
};

} //namespace imEngine

#endif // WIDGET_H
