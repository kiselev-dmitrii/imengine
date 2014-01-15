#ifndef BUTTON_H
#define BUTTON_H

#include "TexturedWidget.h"
#include "Signal.h"
#include <functional>

namespace imEngine {


/** @brief Класс, представляющий собой растягиваемую кнопку
 */
class Button : public BothStretchableTexturedWidget {
public:
        /// Конструктор
        /// Принимает имена картинок, соответствующие состояниям кнопки
        Button(const String& active, const String& hover, const String& pressed,
               const String& disabled, const String& focused, WidgetAbstract* parent = nullptr);

        /// Изменяем состояние в соответствии с действиями мыши
        bool    onMouseEnter(int x, int y);
        bool    onMouseLeave(int x, int y);
        bool    onMousePress(int x, int y, char button);
        bool    onMouseRelease(int x, int y, char button);

public:
        /// Сигналы
        Signal<Button*> onClick;

private:
        String  m_activeImage;
        String  m_hoverImage;
        String  m_pressedImage;
        String  m_disabledImage;
        String  m_focusedImage;

        bool    m_wasPressed;
};


} //namespace imEngine

#endif // BUTTON_H
