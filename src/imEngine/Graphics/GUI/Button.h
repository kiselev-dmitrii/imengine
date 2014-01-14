#ifndef BUTTON_H
#define BUTTON_H

#include "TexturedWidget.h"

namespace imEngine {


/** @brief Класс, представляющий собой кнопку без текста
 */
class Button : public BothStretchableTexturedWidget {
public:
        Button(const String& normalState, const String& hoverState,
               const String& pushedState, const String& disabledState, const String& focusedState);
};


/** @brief Класс, представляющий собой нерастягиваемую кнопку
 */
class ImageButton : public NonStretchableTexturedWidget {
public:
};


} //namespace imEngine

#endif // BUTTON_H
