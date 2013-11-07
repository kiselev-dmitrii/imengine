#include <imEngine/System/Filesystem.h>
#include <imEngine/Utils/Types.h>
#include <imEngine/Utils/Debug.h>
#include <imEngine/Utils/StringUtils.h>
#include <regex>

using namespace imEngine;

String getPathFromInclude(const String& includeString) {
        int start = includeString.find("\"") + 1;
        int end = includeString.find("\"", start) - 1;

        return includeString.substr(start, end - start + 1);
}

String resolveIncludes(const String& source, const String& sourceDirPath) {
        StringList lines = StringUtils::split(source, "\n");

        for (String &line: lines) {
                if (line.find("#include") != String::npos) {
                        auto includePath = Filesystem::joinPath(sourceDirPath, getPathFromInclude(line));
                        IM_LOG("Adding include file: " << includePath);

                        auto includeSource = Filesystem::readTextFile(includePath);
                        if (includeSource == "") {
                                IM_ERROR("Can't open filename " << includePath);
                                return "";
                        }

                        line = resolveIncludes(includeSource, Filesystem::parentPath(includePath));
                }
        }
        return StringUtils::join(lines, "\n");
}

int main() {
        String filename = "resources/shaders/light/phong.glsl";

        String source = Filesystem::readTextFile(filename);
        if (source == "") {
                IM_ERROR("Cannot open file " << filename);
                return 1;
        }
        String processedSource = resolveIncludes(source, Filesystem::parentPath(filename));
        IM_PRINT(processedSource);

        return 0;
}
