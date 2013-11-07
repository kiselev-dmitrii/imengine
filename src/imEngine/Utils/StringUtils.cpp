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

} //namespace imEngine
