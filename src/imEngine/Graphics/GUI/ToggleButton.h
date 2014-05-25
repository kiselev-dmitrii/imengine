#ifndef TOGGLEBUTTON_H
#define TOGGLEBUTTON_H

#include "TexturedWidget.h"
#include "Signal.h"

namespace imEngine {

/** @brief Кнопка, которая имеет два активных состояния:
 *  активное (active) и нажатое (checked)
 */
class ToggleButton : public BothStretchableTexturedWidget {
public:
        /// Конструктор.
        /// Принимает имена картинок для состояний кнопки
        ToggleButton(const String& active, const String& checked, const String& disabledActive,
                     const String& disabledChecked, WidgetAbstract* parent);

        /// Загружает настройки из XML-ноды
        void    loadFromXml(const XmlNode &node);

        /// Устанавливает/возвращает нажатое состояние кнопки
        void    setChecked(bool checked);
        bool    isChecked() const;

        /// Изменяем состояние в соответствии с событиями мыши
        bool    onMousePress(int x, int y, char button);
        bool    onMouseRelease(int x, int y, char button);
        void    onGlobalMouseRelease(int x, int y, char button);

public:
        /// Сигналы
        Signal<ToggleButton*>   onClick;

private:
        String  m_activeImage;
        String  m_checkedImage;
        String  m_disabledActiveImage;
        String  m_disabledCheckedImage;

        bool    m_isPressed;
        bool    m_isChecked;

};

} //namespace imEngine

#endif // TOGGLEBUTTON_H
