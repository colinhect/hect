include_directories(
    "${PROJECT_SOURCE_DIR}/Hect/Source"
    "${PROJECT_SOURCE_DIR}/Dependencies/Catch"
    )
    
FILE(GLOB systemtests_files Source/*.cpp Source/*.h Source/*.inl)
source_group(Source FILES ${systemtests_files})
    
add_executable(HectSystemTests
    "Source/EncodingTests.cpp"
    "Source/GraphicsContextTests.cpp"
    "Source/Main.cpp"
    "Source/SceneTests.cpp"
    )
    
target_link_libraries(HectSystemTests Hect)
    
set_target_properties(HectSystemTests PROPERTIES
    PROJECT_LABEL HectSystemTests
    LINKER_LANGUAGE CXX
    FOLDER "/Hect/Tests"
    )