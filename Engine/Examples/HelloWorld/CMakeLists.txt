project(HelloWorld CXX)

set(SETTINGS_FILES
    "Settings/HelloWorldSettings.yaml"
    )

source_group("Settings" FILES
    ${SETTINGS_FILES}
    )

set(SOURCE_FILES
    "Source/Main.cpp"
    )

source_group("Source" FILES
    ${SOURCE_FILES}
    )

add_executable(HelloWorld ${SOURCE_FILES} ${SETTINGS_FILES})

if(MSVC)
    target_compile_options(HelloWorld PRIVATE /W3 /WX /wd /bigobj)
else()
    target_compile_options(HelloWorld PRIVATE -std=c++1y -Wall -Wextra -Werror -pedantic)
endif()

target_link_libraries(HelloWorld PRIVATE Hect)

set_target_properties(HelloWorld PROPERTIES
    PROJECT_LABEL HelloWorld
    LINKER_LANGUAGE CXX
    FOLDER "/Engine/Examples"
    )

add_custom_command(TARGET HelloWorld
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/Settings ${HECT_OUTPUT_DIR}
    )
