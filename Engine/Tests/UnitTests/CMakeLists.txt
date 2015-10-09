project(HectUnitTests CXX)

if(UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -Wall -Wextra -Werror -pedantic")
endif()

include_directories(
    "${PROJECT_SOURCE_DIR}/../../Source"
    "${PROJECT_SOURCE_DIR}/../../Dependencies/Catch"
    )
    
set(SOURCE_FILES
    "Source/AngleTests.cpp"
    "Source/AnyTests.cpp"
    "Source/AssetTests.cpp"
    "Source/ColorTests.cpp"
    "Source/DataValueTests.cpp"
    "Source/EncodingTests.cpp"
    "Source/EventTests.cpp"
    "Source/FormatTests.cpp"
    "Source/FrameBufferTests.cpp"
    "Source/FrustumTests.cpp"
    "Source/ImageTests.cpp"
    "Source/Main.cpp"
    "Source/MaterialTests.cpp"
    "Source/Matrix4Tests.cpp"
    "Source/MeshReaderTests.cpp"
    "Source/MeshTests.cpp"
    "Source/PathTests.cpp"
    "Source/PlaneTests.cpp"
    "Source/QuaternionTests.cpp"
    "Source/RectangleTests.cpp"
    "Source/ShaderTests.cpp"
    "Source/StreamTests.cpp"
    "Source/StringMapTests.cpp"
    "Source/TaskPoolTests.cpp"
    "Source/TimeSpanTests.cpp"
    "Source/Utilities.cpp"
    "Source/Utilities.h"
    "Source/Vector2Tests.cpp"
    "Source/Vector3Tests.cpp"
    "Source/Vector4Tests.cpp"
    "Source/VertexAttributeTests.cpp"
    "Source/VertexLayoutTests.cpp"
    )
    
source_group("Source" FILES
    "Source/AngleTests.cpp"
    "Source/AnyTests.cpp"
    "Source/AssetTests.cpp"
    "Source/ColorTests.cpp"
    "Source/DataValueTests.cpp"
    "Source/EncodingTests.cpp"
    "Source/EventTests.cpp"
    "Source/FormatTests.cpp"
    "Source/FrameBufferTests.cpp"
    "Source/FrustumTests.cpp"
    "Source/ImageTests.cpp"
    "Source/Main.cpp"
    "Source/MaterialTests.cpp"
    "Source/Matrix4Tests.cpp"
    "Source/MeshReaderTests.cpp"
    "Source/MeshTests.cpp"
    "Source/PathTests.cpp"
    "Source/PlaneTests.cpp"
    "Source/QuaternionTests.cpp"
    "Source/RectangleTests.cpp"
    "Source/ShaderTests.cpp"
    "Source/StreamTests.cpp"
    "Source/StringMapTests.cpp"
    "Source/TaskPoolTests.cpp"
    "Source/TimeSpanTests.cpp"
    "Source/Utilities.cpp"
    "Source/Utilities.h"
    "Source/Vector2Tests.cpp"
    "Source/Vector3Tests.cpp"
    "Source/Vector4Tests.cpp"
    "Source/VertexAttributeTests.cpp"
    "Source/VertexLayoutTests.cpp"
    )

add_executable(HectUnitTests ${SOURCE_FILES})
    
target_link_libraries(HectUnitTests Hect)
    
set_target_properties(HectUnitTests PROPERTIES
    PROJECT_LABEL HectUnitTests
    LINKER_LANGUAGE CXX
    FOLDER "/Engine/Tests"
    )
