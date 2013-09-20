include_directories(${imEngineIncludeDirs})
include_directories(${PROJECT_SOURCE_DIR}/src)

###### imCore SOURCES AND HEADERS ######################################
set(imCoreSources
        ### GRAPHICS BASED ON OPENGL ###
        imCore/Graphics/Texture/Texture.cpp
        imCore/Graphics/Texture/Texture1D.cpp
        imCore/Graphics/Texture/Texture2D.cpp
        imCore/Graphics/Texture/Texture3D.cpp
        imCore/Graphics/Texture/CubeTexture.cpp
        imCore/Graphics/Texture/Image.cpp

        imCore/Graphics/Buffer/BufferObject.cpp
        imCore/Graphics/Buffer/VertexBuffer.cpp
        imCore/Graphics/Buffer/IndexBuffer.cpp
        imCore/Graphics/Buffer/UniformBuffer.cpp

        ### FILE RESOURCES ###
        imCore/FileResource/Mesh.cpp
        imCore/FileResource/Font.cpp

        ### MATHEMATICS ###
        imCore/Math/AABB.cpp
        imCore/Math/Noise.cpp

        ### SYSTEM OPERATIONS ###
        imCore/System/Window.cpp
        imCore/System/Keyboard.cpp
        imCore/System/Mouse.cpp
        imCore/System/BaseApplication.cpp

        ### UTILS ###
        imCore/Utils/StreamUtils.cpp
        imCore/Utils/GLUtils.cpp
        imCore/Utils/Debug.cpp
   )
set(imCoreHeaders
        ### GRAPHICS BASED ON OPENGL ###
        imCore/Graphics/Texture/Texture.h
        imCore/Graphics/Texture/Texture1D.h
        imCore/Graphics/Texture/Texture2D.h
        imCore/Graphics/Texture/Texture3D.h
        imCore/Graphics/Texture/CubeTexture.h
        imCore/Graphics/Texture/Image.h

        imCore/Graphics/Buffer/BufferObject.h
        imCore/Graphics/Buffer/VertexBuffer.h
        imCore/Graphics/Buffer/IndexBuffer.h
        imCore/Graphics/Buffer/UniformBuffer.h

        ### FILE RESOURCES ###
        imCore/FileResource/Mesh.h
        imCore/FileResource/Font.h

        ### MATHEMATICS ###
        imCore/Math/Common.h
        imCore/Math/AABB.h
        imCore/Math/Noise.h

        ### SYSTEM OPERATIONS ###
        imCore/System/Window.h
        imCore/System/Keyboard.h
        imCore/System/Mouse.cpp
        imCore/System/BaseApplication.h

        ### UTILS ###
        imCore/Utils/Uncopyable.h
        imCore/Utils/Types.h
        imCore/Utils/StreamUtils.h
        imCore/Utils/GLUtils.h
        imCore/Utils/Debug.h
   )
#################################################################


###### imFramework SOURCES and HEADERS ##########################
#set(imFrameworkHeaders
#)
#set(imFrameworkSources
#)
#################################################################

add_library(${PROJECT_NAME} SHARED ${imCoreSources} ${imCoreHeaders})
target_link_libraries(imEngine ${imEngineLibs})
