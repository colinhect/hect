# Engine/Tests/UnitTests

include_directories(
    "${PROJECT_SOURCE_DIR}/Engine/Source"
    "${PROJECT_SOURCE_DIR}/Engine/Dependencies/Catch"
    )
    
file(GLOB unittests_files Source/*.cpp Source/*.h Source/*.inl)
source_group(Source FILES ${unittests_files})
    
add_executable(HectUnitTests
    "Source/AngleTests.cpp"
    "Source/AnyTests.cpp"
    "Source/EncodingTests.cpp"
    "Source/EventTests.cpp"
    "Source/FileSystemTests.cpp"
    "Source/FormatTests.cpp"
    "Source/FrustumTests.cpp"
    "Source/JsonTests.cpp"
    "Source/Main.cpp"
    "Source/MaterialTests.cpp"
    "Source/Matrix4Tests.cpp"
    "Source/MeshReaderTests.cpp"
    "Source/MeshTests.cpp"
    "Source/PathTests.cpp"
    "Source/PlaneTests.cpp"
    "Source/QuaternionTests.cpp"
    "Source/StreamTests.cpp"
    "Source/TaskPoolTests.cpp"
    "Source/TimeSpanTests.cpp"
    "Source/Utilities.cpp"
    "Source/Utilities.h"
    "Source/Vector2Tests.cpp"
    "Source/Vector3Tests.cpp"
    "Source/Vector4Tests.cpp"
    "Source/VertexAttributeTests.cpp"
    "Source/VertexLayoutTests.cpp"
    "Source/WorldTests.cpp"
    )
    
target_link_libraries(HectUnitTests Hect)
    
set_target_properties(HectUnitTests PROPERTIES
    PROJECT_LABEL HectUnitTests
    LINKER_LANGUAGE CXX
    FOLDER "/Engine/Tests"
    )