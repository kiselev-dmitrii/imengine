#include "StringUtils.h"
#include <boost/algorithm/string/regex.hpp>
#include <boost/regex.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/algorithm/string/classification.hpp>

namespace imEngine {

StringList StringUtils::split(const String &string, const String &separator) {
        StringList result;
        boost::algorithm::split_regex(result, string, boost::regex(separator));
        return result;
}

String StringUtils::join(const StringList &stringList, const String &separator) {
        return boost::algorithm::join(stringList, separator);
}

Vec4 StringUtils::toVec4(const String &str) {
        StringList list = StringUtils::split(str, ",");
        return Vec4(std::stof(list[0]),
                    std::stof(list[1]),
                    std::stof(list[2]),
                    std::stof(list[3]));
}

Vec3 StringUtils::toVec3(const String &str) {
        StringList list = StringUtils::split(str, ",");
        return Vec3(std::stof(list[0]),
                    std::stof(list[1]),
                    std::stof(list[2]));
}

Vec2 StringUtils::toVec2(const String &str) {
        StringList list = StringUtils::split(str, ",");
        return Vec2(std::stof(list[0]),
                    std::stof(list[1]));
}

Vec4 StringUtils::toColor4(const String &str) {
        return toVec4(str)/255.0f;
}

Vec3 StringUtils::toColor3(const String &str) {
        return toVec3(str)/255.0f;
}

} //namespace imEngine
