#include "Filesystem.h"
#include <boost/filesystem/path.hpp>
#include <fstream>
#include "../Utils/Debug.h"

using namespace boost;

namespace imCore {

String Filesystem::parentPath(const String &path) {
        return filesystem::path(path).parent_path().string();
}

String Filesystem::readTextFile(const String &path) {
        std::ifstream in(path);
        if (!in.is_open()) {
                IM_ERROR("Can't open the file " << path);
                return "";
        }

        String result;
        result.assign(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
        return result;
}

String Filesystem::joinPath(const String &path1, const String &path2) {
        return path1 + "/" + path2;;
}

} //namespace imCore
