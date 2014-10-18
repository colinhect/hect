include_directories(
    "${PROJECT_SOURCE_DIR}/Engine"
    "${PROJECT_SOURCE_DIR}/Dependencies/Catch"
    )
    
FILE(GLOB systemtests_files *.cpp *.h *.inl)
source_group(SystemTests FILES ${systemtests_files})
    
add_executable(HectSystemTests
    "EncodingTests.cpp"
    "GraphicsContextTests.cpp"
    "Main.cpp"
    "SceneTests.cpp"
    )
    
target_link_libraries(HectSystemTests Hect)
    
set_target_properties(HectSystemTests PROPERTIES
    PROJECT_LABEL HectSystemTests
    LINKER_LANGUAGE CXX
    FOLDER "/Hect/Tests"
    )