project(HectUnitTests CXX)

set(SOURCE_FILES
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
    "Source/NameTests.cpp"
    "Source/OptionalTests.cpp"
    "Source/PathTests.cpp"
    "Source/PlaneTests.cpp"
    "Source/QuaternionTests.cpp"
    "Source/RectangleTests.cpp"
    "Source/ShaderTests.cpp"
    "Source/StreamTests.cpp"
    "Source/TaskPoolTests.cpp"
    "Source/UnitConversionTests.cpp"
    "Source/Utilities.cpp"
    "Source/Utilities.h"
    "Source/Vector2Tests.cpp"
    "Source/Vector3Tests.cpp"
    "Source/Vector4Tests.cpp"
    "Source/VertexAttributeTests.cpp"
    "Source/VertexLayoutTests.cpp"
    )

source_group("Source" FILES
    ${SOURCE_FILES}
    )

add_executable(HectUnitTests ${SOURCE_FILES})

if(MSVC)
    target_compile_options(HectUnitTests PRIVATE /W3 /WX /wd /bigobj)
else()
    target_compile_options(HectUnitTests PRIVATE -std=c++1y -Wall -Wextra -Werror -pedantic)
endif()

target_link_libraries(HectUnitTests PRIVATE Hect Catch)

set_target_properties(HectUnitTests PROPERTIES
    PROJECT_LABEL HectUnitTests
    LINKER_LANGUAGE CXX
    FOLDER "/Engine/Tests"
    )

add_test(NAME HectUnitTests
    WORKING_DIRECTORY ${HECT_OUTPUT_DIR}
    COMMAND HectUnitTests)
