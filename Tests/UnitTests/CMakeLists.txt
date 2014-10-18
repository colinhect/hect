include_directories(
    "${PROJECT_SOURCE_DIR}/Engine"
    "${PROJECT_SOURCE_DIR}/Dependencies/Catch"
    )
    
FILE(GLOB unittests_files *.cpp *.h *.inl)
source_group(UnitTests FILES ${unittests_files})
    
add_executable(HectUnitTests
    "AngleTests.cpp"
    "AnyTests.cpp"
    "EncodingTests.cpp"
    "EventTests.cpp"
    "FileSystemTests.cpp"
    "FormatTests.cpp"
    "FrustumTests.cpp"
    "JsonTests.cpp"
    "Main.cpp"
    "MaterialTests.cpp"
    "Matrix4Tests.cpp"
    "MeshReaderTests.cpp"
    "MeshTests.cpp"
    "PathTests.cpp"
    "PlaneTests.cpp"
    "QuaternionTests.cpp"
    "StreamTests.cpp"
    "TaskPoolTests.cpp"
    "TimeSpanTests.cpp"
    "Utilities.cpp"
    "Utilities.h"
    "Vector2Tests.cpp"
    "Vector3Tests.cpp"
    "Vector4Tests.cpp"
    "VertexAttributeTests.cpp"
    "VertexLayoutTests.cpp"
    "WorldTests.cpp"
    )
    
target_link_libraries(HectUnitTests Hect)
    
set_target_properties(HectUnitTests PROPERTIES
    PROJECT_LABEL HectUnitTests
    LINKER_LANGUAGE CXX
    FOLDER "/Hect/Tests"
    )