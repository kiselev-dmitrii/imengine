cmake_minimum_required(VERSION 2.8)	
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

#Имя проекта (PROJECT_NAME=imCore)
project(imCore)

#################################################################
##### Библиотеки ################################################
# OpenGL
find_package(OpenGL REQUIRED)
if(NOT OPENGL_FOUND)
    message(ERROR " OPENGL not found!")
    return()
else()
	include_directories(${OPENGL_INCLUDE_DIRS})
	set(LIBS ${LIBS} ${OPENGL_LIBRARIES})
endif()

# GLEW 
find_package(GLEW REQUIRED)
if(NOT GLEW_FOUND)
	message(ERROR " GLEW not found!")
	return()
else()
	include_directories(${GLEW_INCLUDE_DIRS})
	set(LIBS ${LIBS} ${GLEW_LIBRARIES})
endif()

# DevIL 
find_package(DevIL REQUIRED)
if(NOT IL_FOUND)
	message(ERROR " DevIL not found!")
	return()
else()
	include_directories(${DEVIL_INCLUDE_DIR})
	set(LIBS ${LIBS} ${IL_LIBRARIES})
	set(LIBS ${LIBS} ${ILU_LIBRARIES})
endif()

# Assimp
find_package(Assimp REQUIRED)
if(NOT ASSIMP_FOUND)
	message(ERROR " Assimp not found!")
	return()
else()
	include_directories(${ASSIMP_INCLUDE_DIR})
	set(LIBS ${LIBS} ${ASSIMP_LIBRARIES})
endif()

# GLM
find_package(GLM REQUIRED)
if(NOT GLM_FOUND)
	message(ERROR " GLM not found!")
	return()
else()
	include_directories(${GLM_INCLUDE_DIRS})
endif()

#Линковка
target_link_libraries(${TARGET} ${LIBS}) 
#################################################################

#Флаги
add_definitions(-std=c++11)		#C++11
add_definitions(-Wall)			#Все предупреджения

#Список исходников
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

#Компиляция динамической библиотеки
#add_library(${PROJECT_NAME} SHARED ${SOURCES} ${HEADERS})

