include_directories(${imEngineIncludeDirs})
include_directories(${PROJECT_SOURCE_DIR}/src)

set(TESTS
        WindowTest
        DebugTest
        BaseApplicationTest
)

foreach(TEST ${TESTS})
        add_executable(${TEST} ${TEST}.cpp)
        target_link_libraries(${TEST} imEngine)
endforeach(TEST)
