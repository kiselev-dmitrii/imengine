cmake_minimum_required(VERSION 2.8)	
project(imCore)

###### VARIABLES ################################################
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
if(WIN32)
	set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "deps/win32/include/")
	set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "deps/win32/lib/")
endif()
#################################################################

###### LIBRARIES ################################################
# Here determines the values of the varibales imCoreIncludeDirs and imCoreLibs,
# which contains paths to extarnal libs (to *.h and *.a respectively) 

# OpenGL
find_package(OpenGL REQUIRED)
if(NOT OPENGL_FOUND)
    message(ERROR " OPENGL not found!")
    return()
else()
	list(APPEND imCoreIncludeDirs ${OPENGL_INCLUDE_DIRS})
	list(APPEND imCoreLibs ${OPENGL_LIBRARIES})
endif()

# GLEW 
find_package(GLEW REQUIRED)
if(NOT GLEW_FOUND)
	message(ERROR " GLEW not found!")
	return()
else()
	list(APPEND imCoreIncludeDirs ${GLEW_INCLUDE_DIRS})
	list(APPEND imCoreLibs ${GLEW_LIBRARIES})
endif()

# DevIL 
find_package(DevIL REQUIRED)
if(NOT IL_FOUND)
	message(ERROR " DevIL not found!")
	return()
else()
	list(APPEND imCoreIncludeDirs ${DEVIL_INCLUDE_DIR})
	list(APPEND imCoreLibs ${IL_LIBRARIES})
	list(APPEND imCoreLibs ${ILU_LIBRARIES})
endif()

# Assimp
find_package(Assimp REQUIRED)
if(NOT ASSIMP_FOUND)
	message(ERROR " Assimp not found!")
	return()
else()
	list(APPEND imCoreIncludeDirs ${ASSIMP_INCLUDE_DIR})
	list(APPEND imCoreLibs ${ASSIMP_LIBRARIES})
endif()

# GLM
find_package(GLM REQUIRED)
if(NOT GLM_FOUND)
	message(ERROR " GLM not found!")
	return()
else()
	list(APPEND imCoreIncludeDirs ${GLM_INCLUDE_DIRS})
endif()
#################################################################

###### COMPILER #################################################
if (MINGW OR CMAKE_COMPILER_IS_GNUCXX)
	add_definitions(-std=c++11)		#C++11
	add_definitions(-Wall)			#All warnings turn on
else() 
	message(ERROR " Support only MinGW or G++")
	return()
endif()
#################################################################


###### SUBDIRS ##################################################
add_subdirectory(src/imCore)
add_subdirectory(tests)
#################################################################


###### DOCUMENTATIONS ###########################################
find_package(Doxygen)
if (DOXYGEN_FOUND)
	add_subdirectory(docs)
endif()
#################################################################

