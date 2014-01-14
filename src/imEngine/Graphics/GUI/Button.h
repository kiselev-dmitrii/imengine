#ifndef BUTTON_H
#define BUTTON_H

#include "TexturedWidget.h"

namespace imEngine {


/** @brief Класс, представляющий собой растягиваемую кнопку
 */
class Button : public BothStretchableTexturedWidget {
public:
        /// Конструктор
        /// Принимает на имена картинок, соответствующие состояниям кнопки
        Button(const String& normal, const String& hover, const String& pushed, const String& disabled, const String& focused);

private:
        String  m_normalImage;
        String  m_hoverImage;
        String  m_pushedImage;
        String  m_disabledImage;
        String  m_focusedImage;
};


/** @brief Класс, представляющий собой нерастягиваемую кнопку
 */
class ImageButton : public NonStretchableTexturedWidget {
public:
};


} //namespace imEngine

#endif // BUTTON_H
