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

        ### SYSTEM OPERATIONS ###
        imCore/System/Window.cpp
        imCore/System/Keyboard.cpp
        imCore/System/Mouse.cpp
        imCore/System/Timer.cpp

        ### DEBUG ###
        imCore/Debug/Logger.cpp
        imCore/Debug/GLErrorMonitor.cpp
        imCore/Debug/Profiler.cpp
        imCore/Debug/ProfilerManager.cpp
   )
set(imCoreHeaders
        ### GRAPHICS BASED ON OPENGL ###
        imCore/Graphics/Texture/Texture.h
        imCore/Graphics/Texture/Texture1D.h
        imCore/Graphics/Texture/Texture2D.h
        imCore/Graphics/Texture/Texture3D.h
        imCore/Graphics/Texture/CubeTexture.h

        ### SYSTEM OPERATIONS ###
        imCore/System/Window.h
        imCore/System/Keyboard.h
        imCore/System/Mouse.cpp
        imCore/System/Timer.h

        ### UTILS ###
        imCore/Utils/Uncopyable.h
        imCore/Utils/Types.h

        ### DEBUG ###
        imCore/Debug/Debug.h
        imCore/Debug/Logger.h
        imCore/Debug/GLErrorMonitor.h
        imCore/Debug/Profiler.h
        imCore/Debug/ProfilerManager.h
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
