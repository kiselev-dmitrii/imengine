include_directories(${imEngineIncludeDirs})
include_directories(${PROJECT_SOURCE_DIR}/src)

set(TESTS
        WindowTest
        DebugTest
        BaseApplicationTest
        GraphicApplicationTest
        Texture1DTest
        Texture2DTest
        Texture3DTest
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
        SceneObjectTest
        MeshTest
        GeometryTest
        SceneTest
        FramebufferTest
        SceneDeferredTest
        CubeTextureTest

        MeshConverter
)

foreach(TEST ${TESTS})
        add_executable(${TEST} ${TEST}.cpp)
        target_link_libraries(${TEST} imEngine)
endforeach(TEST)
