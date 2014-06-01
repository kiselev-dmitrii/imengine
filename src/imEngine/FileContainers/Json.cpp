#include "Json.h"
#include <imEngine/Utils/Debug.h>
#include <fstream>

namespace imEngine {

JsonValue JsonUtils::loadFile(const String &path) {
        Json::Value root;
        Json::Reader reader;

        std::ifstream ifile(path, std::ifstream::binary);
        bool isOk = reader.parse(ifile, root, false);
        if (!isOk) {
                IM_ERROR("Cannot open file " << path << ": " << reader.getFormatedErrorMessages());
                return JsonValue();
        }

        return root;
}

Vec2 JsonUtils::toVec2(const JsonValue &value) {
        IM_ASSERT(value.isArray());
        IM_ASSERT(value.size() == 2);

        Vec2 result;
        result.x = value[0].asFloat();
        result.y = value[1].asFloat();
        return result;
}

Vec3 JsonUtils::toVec3(const JsonValue &value) {
        IM_ASSERT(value.isArray());
        IM_ASSERT(value.size() == 3);

        Vec3 result;
        result.x = value[0].asFloat();
        result.y = value[1].asFloat();
        result.z = value[2].asFloat();
        return result;
}

Vec4 JsonUtils::toVec4(const JsonValue &value) {
        IM_ASSERT(value.isArray());
        IM_ASSERT(value.size() == 4);

        Vec4 result;
        result.x = value[0].asFloat();
        result.y = value[1].asFloat();
        result.z = value[2].asFloat();
        result.w = value[3].asFloat();
        return result;
}

Quat JsonUtils::toQuat(const JsonValue &value) {
        Vec4 vec = toVec4(value);
        return Quat(vec.x, vec.y, vec.z, vec.w);
}

} //namespace imEngine
