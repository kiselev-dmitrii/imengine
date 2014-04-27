#ifndef JSON_H
#define JSON_H

#include <json/json.h>
#include <imEngine/Math/Common.h>

namespace imEngine {

typedef Json::Value JsonValue;
typedef Json::Writer JsonWriter;
typedef Json::Reader JsonReader;

namespace JsonUtils {
        /// Преобразует Json-массив в вектор
        Vec2    toVec2(const JsonValue& value);
        Vec3    toVec3(const JsonValue& value);
        Vec4    toVec4(const JsonValue& value);
}

} //namespace imEngine

#endif // JSON_H
