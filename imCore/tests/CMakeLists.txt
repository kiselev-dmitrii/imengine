include_directories(${imCoreIncludeDirs})
include_directories(${imCore_SOURCE_DIR}/src)

set(TESTS
        TextureTest
        LoggerTest
        TimerTest
)

foreach(TEST ${TESTS})
        add_executable(${TEST} ${TEST}.cpp)
        target_link_libraries(${TEST} imCore)
endforeach(TEST)
