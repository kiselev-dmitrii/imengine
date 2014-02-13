#include "Common.h"
#include <imEngine/Utils/GLUtils.h>
#include <imEngine/Utils/Debug.h>

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

SourceType::Enum InternalFormat::assumeSourceType(InternalFormat::Enum internal) {
        switch (internal) {
                case COLOR_NORM_1_COMP_8_BIT:
                case COLOR_NORM_3_COMP_8_BIT:
                case COLOR_NORM_4_COMP_8_BIT:
                case DEPTH_NORM_1_COMP_16_BIT:
                case DEPTH_NORM_1_COMP_24_BIT:
                case DEPTH_NORM_1_COMP_32_BIT:
                        return SourceType::UBYTE;

                case COLOR_FLOAT_3_COMP_32_BIT:
                case COLOR_FLOAT_4_COMP_32_BIT:
                case DEPTH_FLOAT_1_COMP_32_BIT:
                        return SourceType::FLOAT;

                default:
                        IM_ERROR("Bad InternalFormat");
                        return SourceType::UBYTE;
        }
}

SourceFormat::Enum InternalFormat::assumeSourceFormat(InternalFormat::Enum internal) {
        switch (internal) {
                case COLOR_NORM_1_COMP_8_BIT:
                        return SourceFormat::R;

                case COLOR_NORM_3_COMP_8_BIT:
                case COLOR_FLOAT_3_COMP_32_BIT:
                        return SourceFormat::RGB;

                case COLOR_NORM_4_COMP_8_BIT:
                case COLOR_FLOAT_4_COMP_32_BIT:
                        return SourceFormat::RGBA;

                case DEPTH_FLOAT_1_COMP_32_BIT:
                case DEPTH_NORM_1_COMP_16_BIT:
                case DEPTH_NORM_1_COMP_24_BIT:
                case DEPTH_NORM_1_COMP_32_BIT:
                        return SourceFormat::DEPTH;

                default:
                        IM_ERROR("Bad InternalFormat");
                        return SourceFormat::RGB;
        }
}


} //namespace imEngine
