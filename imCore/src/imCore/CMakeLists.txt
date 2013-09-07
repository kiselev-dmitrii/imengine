include_directories(${imCoreIncludeDirs})

###### SOURCES AND HEADERS ######################################
set(imCoreSources
        Graphics/Texture/Texture.cpp
   )
set(imCoreHeaders
        Graphics/Texture/Texture.h

        Utils/Uncopyable.h
        Utils/Debug.h
   )
#################################################################

add_library(${PROJECT_NAME} SHARED ${imCoreSources} ${imCoreHeaders})
target_link_libraries(imCore ${imCoreLibs})
