#ifndef STRINGUTILS_H
#define STRINGUTILS_H

#include "Types.h"
#include <imEngine/Math/Common.h>

namespace imEngine {

namespace StringUtils {
        /// Делит строку string по разделителю separator
        StringList      split(const String& string, const String& separator);
        /// Соединяет строку string, добавляя разделитель separator
        String          join(const StringList& stringList, const String& separator);
        /// Парсит строку с вектором "x,y,z,w"
        Vec4            toVec4(const String& str);
        Vec3            toVec3(const String& str);
        Vec2            toVec2(const String& str);
        /// Парсит цвет в виде "255,255,255,255" и приводит его к промежутку [0;1]
        Vec4            toColor4(const String& str);
        Vec3            toColor3(const String& str);

        /// Переводит массив байт в строку в base64 и обратно
        String          toBase64(ubyte* bytes, uint length);
        String          fromBase64(const String& string);
}

} //namespace imEngine

#endif // STRINGUTILS_H
