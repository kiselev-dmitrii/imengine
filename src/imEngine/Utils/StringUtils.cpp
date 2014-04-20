#include "StringUtils.h"
#include <boost/algorithm/string/regex.hpp>
#include <boost/regex.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/algorithm/string/classification.hpp>

namespace imEngine {

static const String charsBase64 =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789+/";

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

bool isBase64(ubyte c) {
        return (isalnum(c) || (c == '+') || (c == '/'));
}

String StringUtils::toBase64(ubyte *bytes, uint length) {
        String ret;
        int i = 0;
        int j = 0;
        ubyte char_array_3[3];
        ubyte char_array_4[4];

        while (length--) {
                char_array_3[i++] = *(bytes++);
                if (i == 3) {
                        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
                        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
                        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
                        char_array_4[3] = char_array_3[2] & 0x3f;

                        for(i = 0; (i <4) ; i++) ret += charsBase64[char_array_4[i]];
                        i = 0;
                }
        }

        if (i) {
                for(j = i; j < 3; j++) char_array_3[j] = '\0';

                char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
                char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
                char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
                char_array_4[3] = char_array_3[2] & 0x3f;

                for (j = 0; (j < i + 1); j++) ret += charsBase64[char_array_4[j]];
                while((i++ < 3)) ret += '=';
        }

        return ret;
}

String StringUtils::fromBase64(const String &string) {
        int in_len = string.size();
        int i = 0;
        int j = 0;
        int in_ = 0;
        ubyte char_array_4[4], char_array_3[3];
        String ret;

        while (in_len-- && ( string[in_] != '=') && isBase64(string[in_])) {
                char_array_4[i++] = string[in_]; in_++;
                if (i ==4) {
                        for (i = 0; i <4; i++) char_array_4[i] = charsBase64.find(char_array_4[i]);

                        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
                        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
                        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

                        for (i = 0; (i < 3); i++) ret += char_array_3[i];
                        i = 0;
                }
        }

        if (i) {
                for (j = i; j <4; j++) char_array_4[j] = 0;
                for (j = 0; j <4; j++) char_array_4[j] = charsBase64.find(char_array_4[j]);

                char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
                char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
                char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

                for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
        }

        return ret;
}

} //namespace imEngine
