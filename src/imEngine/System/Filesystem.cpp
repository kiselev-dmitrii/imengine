#include "Filesystem.h"
#include <boost/filesystem/path.hpp>
#include <boost/filesystem.hpp>
#include <fstream>
#include "../Utils/Debug.h"

using namespace boost;

namespace imEngine {

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

StringList Filesystem::tree(const String &directoryPath) {
        StringList result;
        filesystem::path directory(directoryPath);

        if (!filesystem::is_directory(directory)) return result;

        // Копируем из directory в content
        std::vector<filesystem::path> content;
        std::copy(filesystem::directory_iterator(directory), filesystem::directory_iterator(), std::back_inserter(content));

        for(filesystem::path& item: content) {
                if (filesystem::is_directory(item)) {
                        StringList subDirContent = tree(item.string());
                        result.insert(result.end(), subDirContent.begin(), subDirContent.end());
                } else {
                        String filename = item.string();
                        std::replace(filename.begin(), filename.end(), '\\', '/');
                        result.push_back(filename);
                }
        }
        std::sort(result.begin(), result.end());

        return result;
}

} //namespace imEngine
