include_directories(${imCoreIncludeDirs})
include_directories(${imCore_SOURCE_DIR}/src)

add_executable(TextureTest TextureTest.cpp)
add_executable(LoggerTest LoggerTest.cpp)

target_link_libraries(LoggerTest imCore)
