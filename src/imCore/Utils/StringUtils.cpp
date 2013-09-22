#include "StringUtils.h"
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/algorithm/string/classification.hpp>

namespace imCore {

StringList StringUtils::split(const String &string, const String &separator) {
        StringList result;
        boost::algorithm::split(result, string, boost::is_any_of(separator));
        return result;
}

String StringUtils::join(const StringList &stringList, const String &separator) {
        return boost::algorithm::join(stringList, separator);
}

} //namespace imCore
