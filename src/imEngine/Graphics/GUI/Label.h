#ifndef LABEL_H
#define LABEL_H

#include "Widget.h"

namespace imEngine {

class Label : public Widget {
public:
        explicit        Label(Widget* parent = 0);

private:
        const String&   m_text;
};

} //namespace imEngine

#endif // LABEL_H
