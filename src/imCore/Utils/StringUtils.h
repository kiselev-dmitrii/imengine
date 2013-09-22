#ifndef STRINGUTILS_H
#define STRINGUTILS_H

#include "Types.h"

namespace imCore {

namespace StringUtils {
        /// Делит строку string по разделителю separator
        StringList      split(const String& string, const String& separator);
        /// Соединяет строку string, добавляя разделитель separator
        String          join(const StringList& stringList, const String& separator);
}

} //namespace imCore

#endif // STRINGUTILS_H
