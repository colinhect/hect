project(HectSystemTests CXX)

set(SETTINGS_FILES
    "Settings/HectSystemTestsSettings.yaml"
    )

source_group("Settings" FILES
    ${SETTINGS_FILES}
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

add_executable(HectSystemTests ${SOURCE_FILES} ${SETTINGS_FILES})

if(MSVC)
    target_compile_options(HectSystemTests PRIVATE /W3 /WX /wd /bigobj)
else()
    target_compile_options(HectSystemTests PRIVATE -std=c++1y -Wall -Wextra -Werror -pedantic)
endif()

target_link_libraries(HectSystemTests PRIVATE Hect Catch)

set_target_properties(HectSystemTests PROPERTIES
    PROJECT_LABEL HectSystemTests
    LINKER_LANGUAGE CXX
    FOLDER "/Engine/Tests"
    )

add_custom_command(TARGET HectSystemTests
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/Settings ${HECT_OUTPUT_DIR}
    )

add_test(NAME HectSystemTests
    WORKING_DIRECTORY ${HECT_OUTPUT_DIR}
    COMMAND HectSystemTests)
