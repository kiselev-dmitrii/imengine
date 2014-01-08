#ifndef DRAWABLEWIDGET_H
#define DRAWABLEWIDGET_H

#include "Widget.h"

namespace imEngine {

/** @brief Виджет который непосредственно рисуется на экране
 *
 * Виджеты которые не наследуются от данного, представляют собой лишь контейнеры
 * для других виджетов. Данный же тип непосредственно отображаются при рендере.
 * Поэтому для них необходимо держать VBO, начальное изображение и т.д
 */
class DrawableWidget : public Widget {
public:
        /// Конструктор. Получает начальную картинку с которой будет рендерится
        explicit DrawableWidget(const String& initalImage, Widget* parent = nullptr);

        /// Инициализация буфера
        void            initialize(GuiManager *manager);
        /// Отрисовка
        void            render();

protected:

        /// Устанавливает на виджет изображение name
        void            setCurrentImage(const String& imageName);
        /// Возвращает текущее изображение
        String          currentImage() const;

        /// Устанавливает количество элементов из которых состоит виджет
        void            setWidgetElementCount(uint count);

        /// Инициализирует VBO и VAO.
        void            initBuffer();
        /// Обновление VBO
        virtual void    updateBuffer() = 0;

protected:
        VertexBufferPtr m_vbo;
        VertexArrayPtr  m_vao;
        bool            m_isNeedToUpdateBuffer;
        uint            m_widgetElementCount;

        String          m_currentImage;
};


/** @brief Нерастягиваемый отрисовываемый виджет
 */
class NonStretchableWidget : public DrawableWidget {
public:
        /// Конструктор
        explicit NonStretchableWidget(const String &initialImage, Widget* parent = nullptr);

        /// Инициализация размера
        void    initialize(GuiManager* manager);

protected:
        /// Обновление буфера
        void    updateBuffer();

};


/** @brief Растягиваемый по горизонтали виджет
 */
class HStretchableWidget : public DrawableWidget {
public:
        /// Конструктор
        explicit HStretchableWidget(const String &initialImage, Widget *parent);

        /// Инициализация размера
        void    initialize(GuiManager* manager);

        /// Установка ширины
        void    setWidth(float width);
        /// Установка минимальной ширины
        void    setMinimalWidth(float width);
        /// Получение минимальной ширины
        float   minimalWidth() const;

protected:
        /// Обновление буфера с учетом растяжимости виджета
        void    updateBuffer();

private:
        float   m_minimalWidth;

};


/** @brief Растягиваемый только по горизонтали виджет
 */
class VStretchableWidget : public DrawableWidget {
public:
        /// Конструктор
        explicit VStretchableWidget(const String &initialImage, Widget* parent);

        /// Инициализация
        void    initialize(GuiManager *manager);

        /// Установка высоты
        void    setHeight(float height);
        /// Установка минимальной высоты
        void    setMinimalHeight(float height);
        /// Получения минимальной высоты
        float   minimalHeight() const;

protected:
        /// Обновление буфера
        void    updateBuffer();

private:
        float   m_minimalHeight;
};


/** @brief Растягиваемый в обе стороны виджет
 */
class BothStretchableWidget : public DrawableWidget {
public:
        /// Конструктор
        explicit BothStretchableWidget(const String &initialImage, Widget* parent);

        /// Инициализация
        void    initialize(GuiManager *manager);

        /// Установка размеров
        void    setSize(const Vec2& size);
        /// Установка минимальных размеров
        void    setMinimalSize(const Vec2& size);
        /// Получение минимальных размеров
        Vec2    minimalSize() const;

protected:
        /// Обновление буфера
        void    updateBuffer();

private:
        Vec2    m_minimalSize;
};


} //namespace imEngine

#endif // DRAWABLEWIDGET_H
