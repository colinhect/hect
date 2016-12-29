project(HectSystemTests CXX)

if(UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -Wall -Wextra -Werror -pedantic")
endif()

include_directories(
    "${PROJECT_SOURCE_DIR}/../../Source"
    "${PROJECT_SOURCE_DIR}/../../Dependencies/Catch"
    )

set(SOURCE_FILES
    "Source/EncodingTests.cpp"
    "Source/FileSystemTests.cpp"
    "Source/HostTests.cpp"
    "Source/InterfaceSystemTests.cpp"
    "Source/Main.cpp"
    "Source/PhysicsSystemTests.cpp"
    "Source/RendererTests.cpp"
    "Source/SceneTests.cpp"
    "Source/ScriptSystemTests.cpp"
    )

source_group("Source" FILES
    ${SOURCE_FILES}
    )

add_executable(HectSystemTests ${SOURCE_FILES})

target_link_libraries(HectSystemTests Hect)

set_target_properties(HectSystemTests PROPERTIES
    PROJECT_LABEL HectSystemTests
    LINKER_LANGUAGE CXX
    FOLDER "/Engine/Tests"
    )

# Copy settings
add_custom_command(TARGET HectSystemTests
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/Settings ${OUTPUT_DIR}/
    )
