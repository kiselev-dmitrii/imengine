include_directories(${imEngineIncludeDirs})
include_directories(${PROJECT_SOURCE_DIR}/src)

set(TESTS
        WindowTest
        DebugTest
        BaseApplicationTest
        GraphicApplicationTest
        Texture1DTest
        Texture2DTest
        FilesystemTest
        ProgramTest
        VertexBufferTest
        TextureManagerTest
        TreeNodeTest
        FontTest
        TextTest
        LuabindTest
        GuiManagerTest
        ButtonTest
        SignalTest
        BoxLayoutTest
        SceneNodeTest
)

foreach(TEST ${TESTS})
        add_executable(${TEST} ${TEST}.cpp)
        target_link_libraries(${TEST} imEngine)
endforeach(TEST)
