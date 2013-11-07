#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "../Utils/Types.h"

namespace imEngine {

/** @brief Является оберткой к модулю Filesystem в BOOST
 */
namespace Filesystem {
        /// Возвращает родительскую путь файла (или директории)
        String          parentPath(const String& path);

        /// Возвращает содержимое текстового файла path
        String          readTextFile(const String& path);

        /// Соединяет два пути
        String          joinPath(const String& path1, const String& path2);

        /// Возвращает список файлов и подфайлов в данной директории
        StringList      tree(const String& directoryPath);

} //nemespace Fylesystem

} //namespace imEngine

#endif // FILESYSTEM_H
