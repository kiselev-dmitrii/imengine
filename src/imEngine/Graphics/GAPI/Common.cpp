#include "Common.h"
#include <imEngine/Utils/GLUtils.h>

namespace imEngine {


int SourceType::sizeOf(SourceType::Enum type) {
       return GLUtils::sizeOf(type);
}

int SourceFormat::numComponents(SourceFormat::Enum srcFormat) {
        switch (srcFormat) {
                case SourceFormat::BGRA:
                case SourceFormat::RGBA:
                        return 4;

                case SourceFormat::BGR:
                case SourceFormat::RGB:
                        return 3;

                case SourceFormat::RG:
                        return 2;

                case SourceFormat::R:
                case SourceFormat::DEPTH:
                        return 1;

                default:
                        return 0;
        }
}


} //namespace imEngine
