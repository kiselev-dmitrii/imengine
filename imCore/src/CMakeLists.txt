include_directories(${imCoreIncludeDirs})

set(SOURCES
        Graphics/Texture.cpp
   )
set(HEADERS
        Graphics/Texture.h
   )

add_library(${PROJECT_NAME} SHARED ${SOURCES} ${HEADERS})
target_link_libraries(imCore ${imCoreLibs})
