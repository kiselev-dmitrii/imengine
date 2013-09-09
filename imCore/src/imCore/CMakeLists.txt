include_directories(${imCoreIncludeDirs})
include_directories(${imCore_SOURCE_DIR}/src)

###### SOURCES AND HEADERS ######################################
set(imCoreSources
        ### GRAPHICS BASED ON OPENGL ###
        Graphics/Texture/Texture.cpp
        Graphics/Texture/Texture1D.cpp
        Graphics/Texture/Texture2D.cpp
        Graphics/Texture/Texture3D.cpp
        Graphics/Texture/CubeTexture.cpp

        ### SYSTEM OPERATIONS ###
        System/Window.cpp
        System/Keyboard.cpp
        System/Mouse.cpp

        ### UTILS ###
        Utils/Timer.cpp

        ### DEBUG ###
        Debug/Logger.cpp
        Debug/GLErrorMonitor.cpp
        Debug/Profiler.cpp
   )
set(imCoreHeaders
        ### GRAPHICS BASED ON OPENGL ###
        Graphics/Texture/Texture.h
        Graphics/Texture/Texture1D.h
        Graphics/Texture/Texture2D.h
        Graphics/Texture/Texture3D.h
        Graphics/Texture/CubeTexture.h

        ### SYSTEM OPERATIONS ###
        System/Window.h
        System/Keyboard.h
        System/Mouse.cpp

        ### UTILS ###
        Utils/Uncopyable.h
        Utils/Types.h
        Utils/Timer.h

        ### DEBUG ###
        Debug/Debug.h
        Debug/Logger.h
        Debug/GLErrorMonitor.h
        Debug/Profiler.h
   )
#################################################################

add_library(${PROJECT_NAME} SHARED ${imCoreSources} ${imCoreHeaders})
target_link_libraries(imCore ${imCoreLibs})
