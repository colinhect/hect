project(HectSystemTests CXX)

if(UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -Wall -Wl,--no-as-needed")
endif()

include_directories(
    "${PROJECT_SOURCE_DIR}/../../Source"
    "${PROJECT_SOURCE_DIR}/../../Dependencies/Catch"
    )
    
file(GLOB systemtests_files Source/*.cpp Source/*.h Source/*.inl)
source_group(Source FILES ${systemtests_files})
    
add_executable(HectSystemTests
    "Source/EncodingTests.cpp"
    "Source/FileSystemTests.cpp"
    "Source/Main.cpp"
    "Source/RendererTests.cpp"
    "Source/SceneTests.cpp"
    )
    
target_link_libraries(HectSystemTests Hect)
    
set_target_properties(HectSystemTests PROPERTIES
    PROJECT_LABEL HectSystemTests
    LINKER_LANGUAGE CXX
    FOLDER "/Engine/Tests"
    )

# Generate reflection code
add_custom_command(TARGET HectSystemTests
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/Assets ${OUTPUT_DIR}/HectSystemTests.data/
    )

# Copy assets
add_custom_command(TARGET HectSystemTests
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/Config ${OUTPUT_DIR}/
    )
