include_directories(${imEngineIncludeDirs})
include_directories(${PROJECT_SOURCE_DIR}/src)

### RECURSIVELY ADD HEADERS AND SOURCES ###
file(GLOB_RECURSE imEngineHeaders *.h)
file(GLOB_RECURSE imEngineSources *.cpp)

add_library(${PROJECT_NAME} SHARED ${imEngineSources} ${imEngineHeaders})
target_link_libraries(imEngine ${imEngineLibs})

