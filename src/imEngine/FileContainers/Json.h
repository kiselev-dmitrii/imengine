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
        /// Сохраняет ноду в json-файл
        void            saveFile(const JsonValue& node, const String& path);

        /// Преобразует Json-массив в вектор
        Vec2            toVec2(const JsonValue& value);
        Vec3            toVec3(const JsonValue& value);
        Vec4            toVec4(const JsonValue& value);
        /// Преобразует Json-массив в кватернион
        Quat            toQuat(const JsonValue& value);

        /// Преобразует вектор в Json-массив
        JsonValue       fromVec2(const Vec2& vec);
        JsonValue       fromVec3(const Vec3& vec);
        JsonValue       fromVec4(const Vec4& vec);
        JsonValue       fromQuat(const Quat& quat);
}

} //namespace imEngine

#endif // JSON_H
