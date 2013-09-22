#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "../Utils/Types.h"

namespace imCore {

/** @brief Является оберткой к модулю Filesystem в BOOST
 */
namespace Filesystem {
        /// Возвращает родительскую путь файла (или директории)
        String          parentPath(const String& path);

        /// Возвращает содержимое текстового файла path
        String          readTextFile(const String& path);

        /// Соединяет два пути
        String          joinPath(const String& path1, const String& path2);

} //nemespace Fylesystem

} //namespace imCore

#endif // FILESYSTEM_H
