#ifndef WIDGET_H
#define WIDGET_H

#include <imEngine/Graphics/GAPI/GAPI.h>
#include <imEngine/Utils/TreeNode.h>
#include "GuiManager.h"

namespace imEngine {


/** @brief Базовый класс для виджетов
 */
class Widget : public TreeNode {
public:
        /// Конструктор. paret - указатель на родителя
        explicit Widget(Widget* parent = nullptr);

        /// Устанавливает позицию виджета в родительских координатах
        void            setPosition(const Vec2& position);
        /// Возвращает позицию виджета в родительских координатах
        Vec2            position() const;
        /// Возвращает позицию виджета в экранных координатах
        Vec2            absolutePosition();

        /// Возвращает размер виджета
        Vec2            size() const;

        /// Возвращает указатель на менеджера
        GuiManager*     manager() const;

        /// Рендерит себя и детей
        virtual void    render() = 0;

protected:
        /// Устанавливает на виджет изображение name
        void            setCurrentImage(const String& name);
        /// Возвращает текущее изображение
        String          currentImage() const;

        /// Действия при добавлении, удалении дочерних виджетов
        void            onAttachChild(TreeNode* node);
        void            onDetachChild(TreeNode* node);

        /// Рендерит всех детей
        void            renderAllChildren();

        /// Оповещает дочерние виджеты об изменении позиции
        /// Она просто говорит, чтобы дети (да и сам виджет) учитывали, что позиция изменилась
        void            notifyChildren();

        /// Обновляет абсолютную позицию, если это надо
        /// Ее следует вызывать тогда, когда нужно получить позицию в абсолютных координатах
        void            updateAbsolutePosition();

        /// Инициализирует VBO и VAO. Используется в наследуемых классах
        void            initBuffer();

protected:
        GuiManager*     m_manager;

        Vec2            m_position;
        Vec2            m_absolutePosition;
        bool            m_isNeedToUpdateAbsolutePosition;

        String          m_currentImage;

        Vec2            m_size;                                         // размер устанавливается в наслудуемых виджетах
        bool            m_isNeedToUpdateBuffer;                         // указывает, что буфер нужно обновить

        // Это используется в основном в наследуемых виджетах.
        VertexBufferPtr m_vbo;
        VertexArrayPtr  m_vao;
};


/** @brief Нерастягиваемый виджет
 */
class NonStretchableWidget : public Widget {
public:
        /// Конструктор
        explicit        NonStretchableWidget(const String& initImage, Widget* parent = nullptr);

        /// Рендерит виджет c заданной картинкой
        void            render();

protected:
        /// Обновляет VBO перед рендером
        void            updateBuffer();
};


/** @brief Виджет, растягиваемый только по ширине
 */
class HStretchWidget : public Widget {
public:
        /// Конструктор
        explicit        HStretchWidget(Widget* parent = nullptr);

        /// Устанавливает ширину
        void            setWidth(float width);
        /// Устанавливает минимальную ширину
        void            setMinimalWidth(float width);
        /// Возвращает минимальную ширину
        float           minimalWidth();

protected:

protected:
        float           m_minimalWidth;
};

/** @brief Базовый класс для виджетов, которые могут изменять размер (панели, окна)
 */
class WidgetResizable : public Widget {
public:
        /// Конструктор
        explicit WidgetResizable(Widget* parent = nullptr);

        /// Устанавилвает размер виджета
        void            setSize(const Vec2& size);
        /// Устанавливает минимальный размер
        void            setMinimalSize(const Vec2& size)                        { m_minimalSize = size; }
        /// Возвращает мин. размер
        Vec2            minimalSize() const                                     { return m_minimalSize; }

protected:
        Vec2            m_minimalSize;
        bool            m_needToUpdateVBO;
};


} //namespace imEngine

#endif // WIDGET_H
