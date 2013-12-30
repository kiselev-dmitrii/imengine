#include "GLUtils.h"

namespace imEngine {

String GLUtils::convertEnumToString(GLenum enumeration) {
        switch (enumeration) {
                case GL_NONE: 		                return "GL_NONE"; break;

                //Биндинги для BufferObjects
                case GL_ARRAY_BUFFER:                   return "GL_ARRAY_BUFFER"; break;
                case GL_COPY_READ_BUFFER:               return "GL_COPY_READ_BUFFER"; break;
                case GL_COPY_WRITE_BUFFER:              return "GL_COPY_WRITE_BUFFER"; break;
                case GL_DRAW_INDIRECT_BUFFER:           return "GL_DRAW_INDIRECT_BUFFER"; break;
                case GL_ELEMENT_ARRAY_BUFFER:           return "GL_ELEMENT_ARRAY_BUFFER"; break;
                case GL_PIXEL_PACK_BUFFER:              return "GL_PIXEL_PACK_BUFFER"; break;
                case GL_PIXEL_UNPACK_BUFFER:            return "GL_PIXEL_UNPACK_BUFFER"; break;
                case GL_TEXTURE_BUFFER:		  return "GL_TEXTURE_BUFFER"; break;
                case GL_TRANSFORM_FEEDBACK_BUFFER:      return "GL_TRANSFORM_FEEDBACK_BUFFER"; break;
                case GL_UNIFORM_BUFFER:		        return "GL_UNIFORM_BUFFER"; break;

                //Использование BufferObjects
                case GL_STREAM_DRAW:	return "GL_STREAM_DRAW"; break;
                case GL_STREAM_READ:    return "GL_STREAM_READ"; break;
                case GL_STREAM_COPY:    return "GL_STREAM_COPY"; break;
                case GL_STATIC_DRAW:	return "GL_STATIC_DRAW"; break;
                case GL_STATIC_READ:	return "GL_STATIC_READ"; break;
                case GL_STATIC_COPY:	return "GL_STATIC_COPY"; break;
                case GL_DYNAMIC_DRAW:	return "GL_DYNAMIC_DRAW"; break;
                case GL_DYNAMIC_READ:	return "GL_DYNAMIC_READ"; break;
                case GL_DYNAMIC_COPY:	return "GL_DYNAMIC_COPY"; break;


                //FBO targets
                case GL_FRAMEBUFFER:	        return "GL_FRAMEBUFFER"; break;
                case GL_READ_FRAMEBUFFER:	return "GL_READ_FRAMEBUFFER"; break;
                case GL_DRAW_FRAMEBUFFER:	return "GL_DRAW_FRAMEBUFFER"; break;

                //Вложения для FBO
                case GL_DEPTH_ATTACHMENT:	        return "GL_DEPTH_ATTACHMENT"; break;
                case GL_STENCIL_ATTACHMENT:		return "GL_STENCIL_ATTACHMENT"; break;
                case GL_DEPTH_STENCIL_ATTACHMENT:	return "GL_DEPTH_STENCIL_ATTACHMENT"; break;
                case GL_COLOR_ATTACHMENT0:		return "GL_COLOR_ATTACHMENT0"; break;
                case GL_COLOR_ATTACHMENT1:		return "GL_COLOR_ATTACHMENT1"; break;
                case GL_COLOR_ATTACHMENT2:		return "GL_COLOR_ATTACHMENT2"; break;
                case GL_COLOR_ATTACHMENT3:		return "GL_COLOR_ATTACHMENT3"; break;
                case GL_COLOR_ATTACHMENT4:		return "GL_COLOR_ATTACHMENT4"; break;
                case GL_COLOR_ATTACHMENT5:		return "GL_COLOR_ATTACHMENT5"; break;
                case GL_COLOR_ATTACHMENT6:		return "GL_COLOR_ATTACHMENT6"; break;
                case GL_COLOR_ATTACHMENT7:		return "GL_COLOR_ATTACHMENT7"; break;
                case GL_COLOR_ATTACHMENT8:		return "GL_COLOR_ATTACHMENT8"; break;
                case GL_COLOR_ATTACHMENT9:		return "GL_COLOR_ATTACHMENT9"; break;
                case GL_COLOR_ATTACHMENT10:		return "GL_COLOR_ATTACHMENT10"; break;
                case GL_COLOR_ATTACHMENT11:		return "GL_COLOR_ATTACHMENT11"; break;
                case GL_COLOR_ATTACHMENT12:		return "GL_COLOR_ATTACHMENT12"; break;
                case GL_COLOR_ATTACHMENT13:		return "GL_COLOR_ATTACHMENT13"; break;
                case GL_COLOR_ATTACHMENT14:		return "GL_COLOR_ATTACHMENT14"; break;
                case GL_COLOR_ATTACHMENT15:		return "GL_COLOR_ATTACHMENT15"; break;

                //FBO статус:
                case GL_FRAMEBUFFER_COMPLETE:				return "GL_FRAMEBUFFER_COMPLETE"; break;
                case GL_FRAMEBUFFER_UNDEFINED:			        return "GL_FRAMEBUFFER_UNDEFINED"; break;
                case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:	        return "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT"; break;
                case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:	return "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT"; break;
                case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:	        return "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER"; break;
                case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:	        return "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER"; break;
                case GL_FRAMEBUFFER_UNSUPPORTED:			return "GL_FRAMEBUFFER_UNSUPPORTED"; break;
                case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:	        return "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE"; break;
                case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:	        return "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS"; break;

                //Коды ошибок
                case GL_INVALID_ENUM:			return "GL_INVALID_ENUM"; break;
                case GL_INVALID_VALUE:		        return "GL_INVALID_VALUE"; break;
                case GL_INVALID_OPERATION:		return "GL_INVALID_OPERATION"; break;
                case GL_INVALID_FRAMEBUFFER_OPERATION:	return "GL_INVALID_FRAMEBUFFER_OPERATION"; break;
                case GL_OUT_OF_MEMORY:			return "GL_OUT_OF_MEMORY"; break;

                //Типы шейдеров
                case GL_VERTEX_SHADER:		return "GL_VERTEX_SHADER"; break;
                case GL_TESS_CONTROL_SHADER:	return "GL_TESS_CONTROL_SHADER"; break;
                case GL_TESS_EVALUATION_SHADER:	return "GL_TESS_EVALUATION_SHADER"; break;
                case GL_GEOMETRY_SHADER:	return "GL_GEOMETRY_SHADER"; break;
                case GL_FRAGMENT_SHADER:	return "GL_FRAGMENT_SHADER"; break;

                //Внутренние базовые форматы текстур
                case GL_DEPTH_COMPONENT:        return "GL_DEPTH_COMPONENT"; break;
                case GL_DEPTH_STENCIL:		return "GL_DEPTH_STENCIL"; break;
                case GL_RED:			return "GL_RED"; break;
                case GL_RG:			return "GL_RG"; break;
                case GL_RGB:			return "GL_RGB"; break;
                case GL_RGBA:			return "GL_RGBA"; break;

                //Внутренние размерные форматы текстур
                case GL_R8:	        return "GL_RED"; break;
                case GL_R8_SNORM:       return "GL_R8_SNORM"; break;
                case GL_R16:		return "GL_R16"; break;
                case GL_R16_SNORM:	return "GL_R16_SNORM"; break;
                case GL_RG8:		return "GL_RG8"; break;
                case GL_RG8_SNORM:	return "GL_RG8_SNORM"; break;
                case GL_RG16:		return "GL_RG16"; break;
                case GL_RG16_SNORM:	return "GL_RG16_SNORM"; break;
                case GL_R3_G3_B2:	return "GL_R3_G3_B2"; break;
                case GL_RGB4:		return "GL_RGB4"; break;
                case GL_RGB5:		return "GL_RGB5"; break;
                case GL_RGB8:		return "GL_RGB8"; break;
                case GL_RGB10:		return "GL_RGB10"; break;
                case GL_RGB12:		return "GL_RGB12"; break;
                case GL_RGB16_SNORM:	return "GL_RGB16_SNORM"; break;
                case GL_RGBA2:		return "GL_RGBA2"; break;
                case GL_RGBA4:		return "GL_RGBA4"; break;
                case GL_RGB5_A1:	return "GL_RGB5_A1"; break;
                case GL_RGBA8:		return "GL_RGBA8"; break;
                case GL_RGBA8_SNORM:	return "GL_RGBA8_SNORM"; break;
                case GL_RGB10_A2:	return "GL_RGB10_A2"; break;
                case GL_RGB10_A2UI:	return "GL_RGB10_A2UI"; break;
                case GL_RGBA12:		return "GL_RGBA12"; break;
                case GL_RGBA16:		return "GL_RGBA16"; break;
                case GL_SRGB8:		return "GL_SRGB8"; break;
                case GL_SRGB8_ALPHA8:	return "GL_SRGB8_ALPHA8"; break;
                case GL_R16F:		return "GL_R16F"; break;
                case GL_RG16F:		return "GL_RG16F"; break;
                case GL_RGB16F:		return "GL_RGB16F"; break;
                case GL_RGBA16F:	return "GL_RGBA16F"; break;
                case GL_R32F:		return "GL_R32F"; break;
                case GL_RG32F:		return "GL_RG32F"; break;
                case GL_RGB32F:		return "GL_RGB32F"; break;
                case GL_RGBA32F:	return "GL_RGBA32F"; break;
                case GL_R11F_G11F_B10F:	return "GL_R11F_G11F_B10F"; break;
                case GL_RGB9_E5:	return "GL_RGB9_E5"; break;
                case GL_R8I:		return "GL_R8I"; break;
                case GL_R8UI:		return "GL_R8UI"; break;
                case GL_R16I:		return "GL_R16I"; break;
                case GL_R16UI:		return "GL_R16UI"; break;
                case GL_R32I:		return "GL_R32I"; break;
                case GL_R32UI:		return "GL_R32UI"; break;
                case GL_RG8I:		return "GL_RG8I"; break;
                case GL_RG8UI:		return "GL_RG8UI"; break;
                case GL_RG16I:		return "GL_RG16I"; break;
                case GL_RG16UI:		return "GL_RG16UI"; break;
                case GL_RG32I:		return "GL_RG32I"; break;
                case GL_RG32UI:		return "GL_RG32UI"; break;
                case GL_RGB8I:		return "GL_RGB8I"; break;
                case GL_RGB8UI:		return "GL_RGB8UI"; break;
                case GL_RGB16I:		return "GL_RGB16I"; break;
                case GL_RGB16UI:	return "GL_RGB16UI"; break;
                case GL_RGB32I:		return "GL_RGB32I"; break;
                case GL_RGB32UI:	return "GL_RGB32UI"; break;
                case GL_RGBA8I:		return "GL_RGBA8I"; break;
                case GL_RGBA8UI:	return "GL_RGBA8UI"; break;
                case GL_RGBA16I:	return "GL_RGBA16I"; break;
                case GL_RGBA16UI:	return "GL_RGBA16UI"; break;
                case GL_RGBA32I:	return "GL_RGBA32I"; break;
                case GL_RGBA32UI:	return "GL_RGBA32UI"; break;
                case GL_DEPTH_COMPONENT16:      return "GL_DEPTH_COMPONENT16"; break;
                case GL_DEPTH_COMPONENT24:	return "GL_DEPTH_COMPONENT24"; break;
                case GL_DEPTH_COMPONENT32F:	return "GL_DEPTH_COMPONENT32F"; break;

                //Внутренние сжатые форматы текстур:
                case GL_COMPRESSED_RED:	                return "GL_COMPRESSED_RED"; break;
                case GL_COMPRESSED_RG:			return "GL_COMPRESSED_RG"; break;
                case GL_COMPRESSED_RGB:			return "GL_COMPRESSED_RGB"; break;
                case GL_COMPRESSED_RGBA:		return "GL_COMPRESSED_RGBA"; break;
                case GL_COMPRESSED_SRGB:		return "GL_COMPRESSED_SRGB"; break;
                case GL_COMPRESSED_SRGB_ALPHA:		return "GL_COMPRESSED_SRGB_ALPHA"; break;
                case GL_COMPRESSED_RED_RGTC1:		return "GL_COMPRESSED_RED_RGTC1"; break;
                case GL_COMPRESSED_SIGNED_RED_RGTC1:	return "GL_COMPRESSED_SIGNED_RED_RGTC1"; break;
                case GL_COMPRESSED_RG_RGTC2:		return "GL_COMPRESSED_RG_RGTC2"; break;
                case GL_COMPRESSED_SIGNED_RG_RGTC2:	return "GL_COMPRESSED_SIGNED_RG_RGTC2"; break;

                //Texture targets:
                case GL_TEXTURE_1D: 			        return "GL_TEXTURE_1D"; break;
                case GL_PROXY_TEXTURE_1D: 			return "GL_PROXY_TEXTURE_1D"; break;
                case GL_TEXTURE_2D: 				return "GL_TEXTURE_2D"; break;
                case GL_PROXY_TEXTURE_2D: 			return "GL_PROXY_TEXTURE_2D"; break;
                case GL_TEXTURE_3D: 				return "GL_TEXTURE_3D"; break;
                case GL_PROXY_TEXTURE_3D: 			return "GL_PROXY_TEXTURE_3D"; break;
                case GL_TEXTURE_1D_ARRAY: 			return "GL_TEXTURE_1D_ARRAY"; break;
                case GL_PROXY_TEXTURE_1D_ARRAY: 		return "GL_PROXY_TEXTURE_1D_ARRAY"; break;
                case GL_TEXTURE_2D_ARRAY: 			return "GL_TEXTURE_2D_ARRAY"; break;
                case GL_PROXY_TEXTURE_2D_ARRAY: 		return "GL_PROXY_TEXTURE_2D_ARRAY"; break;
                case GL_TEXTURE_RECTANGLE: 			return "GL_TEXTURE_RECTANGLE"; break;
                case GL_PROXY_TEXTURE_RECTANGLE: 		return "GL_PROXY_TEXTURE_RECTANGLE"; break;
                case GL_TEXTURE_CUBE_MAP: 			return "GL_TEXTURE_CUBE_MAP"; break;
                case GL_PROXY_TEXTURE_CUBE_MAP: 		return "GL_PROXY_TEXTURE_CUBE_MAP"; break;
                case GL_TEXTURE_2D_MULTISAMPLE: 		return "GL_TEXTURE_2D_MULTISAMPLE"; break;
                case GL_PROXY_TEXTURE_2D_MULTISAMPLE: 	        return "GL_PROXY_TEXTURE_2D_MULTISAMPLE"; break;
                case GL_TEXTURE_2D_MULTISAMPLE_ARRAY:	        return "GL_TEXTURE_2D_MULTISAMPLE_ARRAY"; break;
                case GL_PROXY_TEXTURE_2D_MULTISAMPLE_ARRAY:	return "GL_PROXY_TEXTURE_2D_MULTISAMPLE_ARRAY"; break;
                case GL_TEXTURE_CUBE_MAP_POSITIVE_X:	        return "GL_CUBE_MAP_POSITIVE_X"; break;
                case GL_TEXTURE_CUBE_MAP_NEGATIVE_X:	        return "GL_CUBE_MAP_NEGATIVE_X"; break;
                case GL_TEXTURE_CUBE_MAP_POSITIVE_Y:	        return "GL_CUBE_MAP_POSITIVE_Y"; break;
                case GL_TEXTURE_CUBE_MAP_NEGATIVE_Y:	        return "GL_CUBE_MAP_NEGATIVE_Y"; break;
                case GL_TEXTURE_CUBE_MAP_POSITIVE_Z:	        return "GL_CUBE_MAP_POSITIVE_Z"; break;
                case GL_TEXTURE_CUBE_MAP_NEGATIVE_Z:	        return "GL_CUBE_MAP_NEGATIVE_Z"; break;

                //Текстурные параметры:
                case GL_TEXTURE_BASE_LEVEL:		        return "GL_TEXTURE_BASE_LEVEL"; break;
                case GL_TEXTURE_COMPARE_FUNC:			return "GL_TEXTURE_COMPARE_FUNC"; break;
                case GL_TEXTURE_COMPARE_MODE:			return "GL_TEXTURE_COMPARE_MODE"; break;
                case GL_TEXTURE_LOD_BIAS:			return "GL_TEXTURE_LOD_BIAS"; break;
                case GL_TEXTURE_MIN_FILTER:			return "GL_TEXTURE_MIN_FILTER"; break;
                case GL_TEXTURE_MAG_FILTER:			return "GL_TEXTURE_MAG_FILTER"; break;
                case GL_TEXTURE_MIN_LOD:			return "GL_TEXTURE_MIN_LOD"; break;
                case GL_TEXTURE_MAX_LOD:			return "GL_TEXTURE_MAX_LOD"; break;
                case GL_TEXTURE_MAX_LEVEL:			return "GL_TEXTURE_MAX_LEVEL"; break;
                case GL_TEXTURE_SWIZZLE_R:			return "GL_TEXTURE_SWIZZLE_R"; break;
                case GL_TEXTURE_SWIZZLE_G:			return "GL_TEXTURE_SWIZZLE_G"; break;
                case GL_TEXTURE_SWIZZLE_B:			return "GL_TEXTURE_SWIZZLE_B"; break;
                case GL_TEXTURE_SWIZZLE_A:			return "GL_TEXTURE_SWIZZLE_A"; break;
                case GL_TEXTURE_WRAP_S:				return "GL_TEXTURE_WRAP_S"; break;
                case GL_TEXTURE_WRAP_T:				return "GL_TEXTURE_WRAP_T"; break;
                case GL_TEXTURE_WRAP_R:				return "GL_TEXTURE_WRAP_R"; break;

                //Текстурные параметры для фильтрования:
                case GL_NEAREST:			        return "GL_NEAREST"; break;
                case GL_LINEAR:					return "GL_LINEAR"; break;
                case GL_NEAREST_MIPMAP_NEAREST:			return "GL_NEAREST_MIPMAP_NEAREST"; break;
                case GL_LINEAR_MIPMAP_NEAREST:			return "GL_LINEAR_MIPMAP_NEAREST"; break;
                case GL_NEAREST_MIPMAP_LINEAR:			return "GL_NEAREST_MIPMAP_LINEAR"; break;
                case GL_LINEAR_MIPMAP_LINEAR:			return "GL_LINEAR_MIPMAP_LINEAR"; break;

                //Типы:
                case GL_UNSIGNED_BYTE:                  return "GL_UNSIGNED_BYTE"; break;
                case GL_BYTE:                           return "GL_BYTE"; break;
                case GL_UNSIGNED_SHORT:                 return "GL_UNSIGNED_SHORT"; break;
                case GL_SHORT:                          return "GL_SHORT"; break;
                case GL_FLOAT:				return "GL_FLOAT"; break;
                case GL_FLOAT_VEC2:			return "GL_FLOAT_VEC2"; break;
                case GL_FLOAT_VEC3:			return "GL_FLOAT_VEC3"; break;
                case GL_FLOAT_VEC4:			return "GL_FLOAT_VEC4"; break;
                case GL_DOUBLE:				return "GL_DOUBLE"; break;
                case GL_DOUBLE_VEC2:			return "GL_DOUBLE_VEC2"; break;
                case GL_DOUBLE_VEC3:			return "GL_DOUBLE_VEC3"; break;
                case GL_DOUBLE_VEC4:			return "GL_DOUBLE_VEC4"; break;
                case GL_INT:				return "GL_INT"; break;
                case GL_INT_VEC2:			return "GL_INT_VEC2"; break;
                case GL_INT_VEC3:			return "GL_INT_VEC3"; break;
                case GL_INT_VEC4:			return "GL_INT_VEC4"; break;
                case GL_UNSIGNED_INT:			return "GL_UNSIGNED_INT"; break;
                case GL_UNSIGNED_INT_VEC2:		return "GL_UNSIGNED_INT_VEC2"; break;
                case GL_UNSIGNED_INT_VEC3:		return "GL_UNSIGNED_INT_VEC3"; break;
                case GL_UNSIGNED_INT_VEC4:		return "GL_UNSIGNED_INT_VEC4"; break;
                case GL_BOOL:				return "GL_BOOL"; break;
                case GL_BOOL_VEC2:			return "GL_BOOL_VEC2"; break;
                case GL_BOOL_VEC3:			return "GL_BOOL_VEC3"; break;
                case GL_BOOL_VEC4:			return "GL_BOOL_VEC4"; break;
                case GL_FLOAT_MAT2:			return "GL_FLOAT_MAT2"; break;
                case GL_FLOAT_MAT3:			return "GL_FLOAT_MAT3"; break;
                case GL_FLOAT_MAT4:			return "GL_FLOAT_MAT4"; break;
                case GL_FLOAT_MAT2x3:			return "GL_FLOAT_MAT2x3"; break;
                case GL_FLOAT_MAT2x4:			return "GL_FLOAT_MAT2x4"; break;
                case GL_FLOAT_MAT3x2:			return "GL_FLOAT_MAT3x2"; break;
                case GL_FLOAT_MAT3x4:			return "GL_FLOAT_MAT3x4"; break;
                case GL_FLOAT_MAT4x2:			return "GL_FLOAT_MAT4x2"; break;
                case GL_FLOAT_MAT4x3:			return "GL_FLOAT_MAT4x3"; break;
                case GL_DOUBLE_MAT2:			return "GL_DOUBLE_MAT2"; break;
                case GL_DOUBLE_MAT3:			return "GL_DOUBLE_MAT3"; break;
                case GL_DOUBLE_MAT4:			return "GL_DOUBLE_MAT4"; break;
                case GL_DOUBLE_MAT2x3:			return "GL_DOUBLE_MAT2x3"; break;
                case GL_DOUBLE_MAT2x4:			return "GL_DOUBLE_MAT2x4"; break;
                case GL_DOUBLE_MAT3x2:			return "GL_DOUBLE_MAT3x2"; break;
                case GL_DOUBLE_MAT3x4:			return "GL_DOUBLE_MAT3x4"; break;
                case GL_DOUBLE_MAT4x2:			return "GL_DOUBLE_MAT4x2"; break;
                case GL_DOUBLE_MAT4x3:			return "GL_DOUBLE_MAT4x3"; break;
                case GL_SAMPLER_1D:			return "GL_SAMPLER_1D"; break;
                case GL_SAMPLER_2D:			return "GL_SAMPLER_2D"; break;
                case GL_SAMPLER_2D_RECT:		return "GL_SAMPLER_2D_RECT"; break;
                case GL_SAMPLER_3D:			return "GL_SAMPLER_3D"; break;
                case GL_SAMPLER_CUBE:			return "GL_SAMPLER_CUBE"; break;
                case GL_SAMPLER_1D_SHADOW:		return "GL_SAMPLER_1D_SHADOW"; break;
                case GL_SAMPLER_2D_SHADOW:		return "GL_SAMPLER_2D_SHADOW"; break;
                case GL_SAMPLER_2D_RECT_SHADOW:		return "GL_SAMPLER_2D_RECT_SHADOW"; break;
                case GL_SAMPLER_CUBE_SHADOW:		return "GL_SAMPLER_CUBE_SHADOW"; break;
                case GL_SAMPLER_1D_ARRAY:		return "GL_SAMPLER_1D_ARRAY"; break;
                case GL_SAMPLER_2D_ARRAY:		return "GL_SAMPLER_2D_ARRAY"; break;
                case GL_SAMPLER_1D_ARRAY_SHADOW:	return "GL_SAMPLER_1D_ARRAY_SHADOW"; break;
                case GL_SAMPLER_2D_ARRAY_SHADOW:	return "GL_SAMPLER_2D_ARRAY_SHADOW"; break;
                case GL_SAMPLER_2D_MULTISAMPLE:	        return "GL_SAMPLER_2D_MULTISAMPLE"; break;
                case GL_SAMPLER_2D_MULTISAMPLE_ARRAY:	return "GL_SAMPLER_2D_MULTISAMPLE_ARRAY"; break;
                case GL_SAMPLER_BUFFER:			return "GL_SAMPLER_BUFFER"; break;
                case GL_INT_SAMPLER_1D:			return "GL_INT_SAMPLER_1D"; break;
                case GL_INT_SAMPLER_2D:			return "GL_INT_SAMPLER_2D"; break;
                case GL_INT_SAMPLER_2D_RECT:		return "GL_INT_SAMPLER_2D_RECT"; break;
                case GL_INT_SAMPLER_3D:			return "GL_INT_SAMPLER_3D"; break;
                case GL_INT_SAMPLER_CUBE:		return "GL_INT_SAMPLER_CUBE"; break;
                case GL_INT_SAMPLER_1D_ARRAY:		return "GL_INT_SAMPLER_1D_ARRAY"; break;
                case GL_INT_SAMPLER_2D_ARRAY:		return "GL_INT_SAMPLER_2D_ARRAY"; break;
                case GL_INT_SAMPLER_2D_MULTISAMPLE:	return "GL_INT_SAMPLER_2D_MULTISAMPLE"; break;
                case GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY:	return "GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY"; break;
                case GL_INT_SAMPLER_BUFFER:			return "GL_INT_SAMPLER_BUFFER"; break;
                case GL_UNSIGNED_INT_SAMPLER_1D:		return "GL_UNSIGNED_INT_SAMPLER_1D"; break;
                case GL_UNSIGNED_INT_SAMPLER_2D:		return "GL_UNSIGNED_INT_SAMPLER_2D"; break;
                case GL_UNSIGNED_INT_SAMPLER_2D_RECT:	        return "GL_UNSIGNED_INT_SAMPLER_2D_RECT"; break;
                case GL_UNSIGNED_INT_SAMPLER_3D:                return "GL_UNSIGNED_INT_SAMPLER_3D"; break;
                case GL_UNSIGNED_INT_SAMPLER_CUBE:              return "GL_UNSIGNED_INT_SAMPLER_CUBE"; break;
                case GL_UNSIGNED_INT_SAMPLER_1D_ARRAY:          return "GL_UNSIGNED_INT_SAMPLER_1D_ARRAY"; break;
                case GL_UNSIGNED_INT_SAMPLER_2D_ARRAY:          return "GL_UNSIGNED_INT_SAMPLER_2D_ARRAY"; break;
                case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE:	return "GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE"; break;
                case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY:	return "GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY"; break;
                case GL_UNSIGNED_INT_SAMPLER_BUFFER:                    return "GL_UNSIGNED_INT_SAMPLER_BUFFER"; break;

                default: break;
        };

        return "Unknown GLenum";
}

} //namespace imEngine
