#ifndef BUTTON_H
#define BUTTON_H

#include "TexturedWidget.h"

namespace imEngine {


/** @brief Класс, представляющий собой растягиваемую кнопку
 */
class Button : public BothStretchableTexturedWidget {
public:
        /// Конструктор
        /// Принимает имена картинок, соответствующие состояниям кнопки
        Button(const String& active, const String& hover, const String& pressed,
               const String& disabled, const String& focused, WidgetAbstract* parent = nullptr);

        /// Обновляет состояние виджета
        void    update()                                                        { }

private:
        String  m_activeImage;
        String  m_hoverImage;
        String  m_pressedImage;
        String  m_disabledImage;
        String  m_focusedImage;
};


} //namespace imEngine

#endif // BUTTON_H
