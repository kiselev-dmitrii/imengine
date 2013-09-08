include_directories(${imCoreIncludeDirs})
include_directories(${imCore_SOURCE_DIR}/src)

###### SOURCES AND HEADERS ######################################
set(imCoreSources
        Graphics/Texture/Texture.cpp

        Utils/Timer.cpp

        Debug/GLErrorMonitor.cpp
        Debug/Logger.cpp
   )
set(imCoreHeaders
        Graphics/Texture/Texture.h

        Utils/Uncopyable.h
        Utils/Types.h
        Utils/Timer.h

        Debug/Debug.h
        Debug/GLErrorMonitor.h
        Debug/Logger.h
   )
#################################################################

add_library(${PROJECT_NAME} SHARED ${imCoreSources} ${imCoreHeaders})
target_link_libraries(imCore ${imCoreLibs})
