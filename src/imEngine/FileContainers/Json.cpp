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

void JsonUtils::saveFile(const JsonValue& node, const String &path) {
        Json::StyledWriter writer;
        std::ofstream ofile(path, std::ofstream::binary);

        ofile << writer.write(node);

        ofile.close();
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

JsonValue JsonUtils::fromVec2(const Vec2 &vec) {
        JsonValue result(Json::arrayValue);
        result.append(vec.x);
        result.append(vec.y);
        return result;
}

JsonValue JsonUtils::fromVec3(const Vec3 &vec) {
        JsonValue result(Json::arrayValue);
        result.append(vec.x);
        result.append(vec.y);
        result.append(vec.z);
        return result;
}

JsonValue JsonUtils::fromVec4(const Vec4 &vec) {
        JsonValue result(Json::arrayValue);
        result.append(vec.x);
        result.append(vec.y);
        result.append(vec.z);
        result.append(vec.w);
        return result;
}

JsonValue JsonUtils::fromQuat(const Quat &quat) {
        JsonValue result(Json::arrayValue);
        result.append(quat.x);
        result.append(quat.y);
        result.append(quat.z);
        result.append(quat.w);
        return result;
}


} //namespace imEngine
