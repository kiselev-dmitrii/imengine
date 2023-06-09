cmake_minimum_required(VERSION 2.8)	
project(imEngine)

###### VARIABLES ################################################
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
if(WIN32)
        set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "deps/win32/include/")
        set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "deps/win32/lib/")
endif()

# All binaries (*.dll and *.exe) will be placed to build/bin directory
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY
        ${PROJECT_BINARY_DIR}/bin
        CACHE PATH
        "Single Directory for all"
    )
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
        ${PROJECT_BINARY_DIR}/bin
        CACHE PATH
        "Single Directory for all"
    )
#################################################################

###### LIBRARIES ################################################
# Here determines the values of the varibales imEngineIncludeDirs and imEngineLibs,
# which contains paths to extarnal libs (to *.h and *.a respectively) 

#SDL2
find_package(SDL2 REQUIRED)
if(NOT SDL2_FOUND)
        message(ERROR " SDL2 not found!")
        return()
else()
        list(APPEND imEngineIncludeDirs ${SDL2_INCLUDE_DIR})
        list(APPEND imEngineLibs ${SDL2_LIBRARY})
endif()

# OpenGL
find_package(OpenGL REQUIRED)
if(NOT OPENGL_FOUND)
        message(ERROR " OPENGL not found!")
        return()
else()
        list(APPEND imEngineIncludeDirs ${OPENGL_INCLUDE_DIRS})
        list(APPEND imEngineLibs ${OPENGL_LIBRARIES})
endif()

# GLEW 
find_package(GLEW REQUIRED)
if(NOT GLEW_FOUND)
        message(ERROR " GLEW not found!")
        return()
else()
        list(APPEND imEngineIncludeDirs ${GLEW_INCLUDE_PATH})
        list(APPEND imEngineLibs ${GLEW_LIBRARY})
endif()

# DevIL
find_package(DevIL REQUIRED)
if(NOT IL_FOUND)
        message(ERROR " DevIL not found!")
        return()
else()
        list(APPEND imEngineIncludeDirs ${IL_INCLUDE_DIR})
        list(APPEND imEngineLibs ${IL_LIBRARIES})
        list(APPEND imEngineLibs ${ILU_LIBRARIES})
endif()

# Assimp
find_package(Assimp REQUIRED)
if(NOT ASSIMP_FOUND)
        message(ERROR " Assimp not found!")
        return()
else()
        list(APPEND imEngineIncludeDirs ${ASSIMP_INCLUDE_DIR})
        list(APPEND imEngineLibs ${ASSIMP_LIBRARIES})
endif()

# GLM
find_package(GLM REQUIRED)
if(NOT GLM_FOUND)
        message(ERROR " GLM not found!")
        return()
else()
        list(APPEND imEngineIncludeDirs ${GLM_INCLUDE_DIRS})
endif()

# BOOST::Filesystem
find_package(Boost COMPONENTS filesystem system regex REQUIRED)
if (NOT Boost_FOUND)
        message(ERROR " Boost::filesystem or Boost::system not found!")
        return()
else()
        list(APPEND imEngineIncludeDirs ${Boost_INCLUDE_DIRS})
        list(APPEND imEngineLibs ${Boost_LIBRARIES})
endif()

# FreeType
find_package(Freetype REQUIRED)
if(NOT FREETYPE_FOUND)
        message(ERROR " Freetype not found!")
        return()
else()
        list(APPEND imEngineIncludeDirs ${FREETYPE_INCLUDE_DIRS})
        list(APPEND imEngineLibs ${FREETYPE_LIBRARIES})
endif()

# JsonCpp
find_package(JsonCpp REQUIRED)
if(NOT JSONCPP_FOUND)
        message(ERROR " JsonCpp not found!")
        return()
else()
        list(APPEND imEngineIncludeDirs ${JSONCPP_INCLUDE_DIRS})
        list(APPEND imEngineLibs ${JSONCPP_LIBRARIES})
endif()

# Luabind
find_package(Luabind REQUIRED)
if(NOT LUABIND_FOUND)
        message(ERROR " Luabind not found")
        return()
else()
        list(APPEND imEngineIncludeDirs ${LUABIND_INCLUDE_DIRS})
        list(APPEND imEngineLibs ${LUABIND_LIBRARIES})
endif()


#################################################################


###### DEFINES ##################################################
add_definitions(-DIM_DEBUG)

###### COMPILER #################################################
if (MINGW OR CMAKE_COMPILER_IS_GNUCXX)
        add_definitions(-std=c++0x)		#C++11
        add_definitions(-Wall)			#All warnings turn on
else() 
        message(ERROR " Support only MinGW or G++")
        return()
endif()
#################################################################


###### SUBDIRS ##################################################
add_subdirectory(src)
add_subdirectory(tests)
#################################################################


###### COPY RESOURCES ###########################################
file(COPY ${PROJECT_SOURCE_DIR}/resources DESTINATION ${PROJECT_BINARY_DIR}/bin/)
#################################################################


###### DOCUMENTATIONS ###########################################
find_package(Doxygen)
if (DOXYGEN_FOUND)
        # It's just copy file from docs/Doxyfile.in to build/docs/Doxyfile
        configure_file(${PROJECT_SOURCE_DIR}/docs/Doxyfile.in
                       ${PROJECT_BINARY_DIR}/docs/Doxyfile @ONLY)
        add_subdirectory(docs)
endif()
#################################################################
