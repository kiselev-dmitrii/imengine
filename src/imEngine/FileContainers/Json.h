#ifndef JSON_H
#define JSON_H

#include <json/json.h>
#include <imEngine/Math/Common.h>
#include <imEngine/Utils/Types.h>

namespace imEngine {

typedef Json::Value JsonValue;
typedef Json::Writer JsonWriter;
typedef Json::Reader JsonReader;

namespace JsonUtils {
        /// Загружает json-файл и возвращает корневую ноду
        JsonValue       loadFile(const String& path);

        /// Преобразует Json-массив в вектор
        Vec2            toVec2(const JsonValue& value);
        Vec3            toVec3(const JsonValue& value);
        Vec4            toVec4(const JsonValue& value);
        /// Преобразует Json-массив в кватернион
        Quat            toQuat(const JsonValue& value);
}

} //namespace imEngine

#endif // JSON_H
