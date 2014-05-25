#ifndef TEXTUREDWIDGET_H
#define TEXTUREDWIDGET_H

#include "Widget.h"

namespace imEngine {

/** @brief Виджет который непосредственно рисуется на экране
 *
 * Виджеты которые не наследуются от данного, представляют собой лишь контейнеры
 * для других виджетов. Данный же тип непосредственно отображаются при рендере.
 * Поэтому для них необходимо держать VBO, начальное изображение и т.д
 */
class TexturedWidgetAbstract : public WidgetAbstract {
public:
        /// Конструктор. Получает начальную картинку с которой будет рендерится
        explicit TexturedWidgetAbstract(const String& initalImage, WidgetAbstract* parent);

        /// Отрисовка виджета
        void            onRender();

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
class NonStretchableTexturedWidget : public TexturedWidgetAbstract {
public:
        /// Конструктор
        explicit NonStretchableTexturedWidget(const String &initialImage, WidgetAbstract* parent);

protected:
        /// Обновление буфера
        void    updateBuffer();

};


/** @brief Растягиваемый по горизонтали виджет
 */
class HStretchableTexturedWidget : public TexturedWidgetAbstract, public HStretchableAbstract {
public:
        /// Конструктор
        explicit HStretchableTexturedWidget(const String &initialImage, WidgetAbstract *parent);

        /// Загружает параметры из XML-ноды
        void    loadFromXml(const XmlNode& node);

        /// Установка ширины
        void    setWidth(float width);

protected:
        /// Обновление буфера с учетом растяжимости виджета
        void    updateBuffer();
};


/** @brief Растягиваемый только по горизонтали виджет
 */
class VStretchableTexturedWidget : public TexturedWidgetAbstract, public VStretchableAbstract {
public:
        /// Конструктор
        explicit VStretchableTexturedWidget(const String &initialImage, WidgetAbstract* parent);

        /// Загружает параметры из XML-ноды
        void    loadFromXml(const XmlNode& node);

        /// Установка высоты
        void    setHeight(float height);

protected:
        /// Обновление буфера
        void    updateBuffer();
};


/** @brief Растягиваемый в обе стороны виджет
 */
class BothStretchableTexturedWidget : public TexturedWidgetAbstract, public BothStretchableAbstract {
public:
        /// Конструктор
        explicit BothStretchableTexturedWidget(const String &initialImage, WidgetAbstract* parent);

        /// Загружает параметры из XML-ноды
        void    loadFromXml(const XmlNode& node);

        /// Установка ширины
        void    setWidth(float width);
        /// Установка высоты
        void    setHeight(float height);

protected:
        /// Обновление буфера
        void    updateBuffer();
};


} //namespace imEngine

#endif // TEXTUREDWIDGET_H
