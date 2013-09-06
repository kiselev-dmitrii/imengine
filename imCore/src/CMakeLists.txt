include_directories(${imCoreIncludeDirs})

set(SOURCES 	Graphics/BufferObject.cpp
				Graphics/Texture.cpp
				Graphics/Texture1D.cpp
				Graphics/Texture2D.cpp

   )
set(HEADERS		Graphics/BufferObject.h
				Graphics/Texture.h
				Graphics/Texture1D.h
				Graphics/Texture2D.h
   )

add_library(${PROJECT_NAME} SHARED ${SOURCES} ${HEADERS})
target_link_libraries(glfw ${imCoreLibs})