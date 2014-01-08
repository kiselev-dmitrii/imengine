#ifndef BUTTON_H
#define BUTTON_H

#include "Widget.h"

namespace imEngine {

class Button : public Widget {
public:
        Button(const String& normalState, const String& hoverState,
               const String& pushedState, const String& disabledState, const String& focusedState);
};


} //namespace imEngine

#endif // BUTTON_H
