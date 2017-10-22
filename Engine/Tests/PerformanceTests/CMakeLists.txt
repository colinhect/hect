project(HectPerformanceTests CXX)

set(SETTINGS_FILES
    "Settings/HectPerformanceTestsSettings.yaml"
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

add_executable(HectPerformanceTests ${SOURCE_FILES} ${SETTINGS_FILES})

if(UNIX)
    target_compile_options(HectPerformanceTests PRIVATE -std=c++1y -Wall -Wextra -Werror -pedantic)
endif()

target_compile_definitions(HectPerformanceTests PRIVATE CELERO_STATIC)
target_link_libraries(HectPerformanceTests PRIVATE Hect Celero)

if(MSVC)
    target_link_libraries(HectPerformanceTests PRIVATE PowrProf.lib)
else()
    target_link_libraries(HectPerformanceTests PRIVATE pthread)
endif()

set_target_properties(HectPerformanceTests PROPERTIES
    PROJECT_LABEL HectPerformanceTests
    LINKER_LANGUAGE CXX
    FOLDER "/Engine/Tests"
    )

add_custom_command(TARGET HectPerformanceTests
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/Settings ${HECT_OUTPUT_DIR}
    )

add_test(NAME HectPerformanceTests
    WORKING_DIRECTORY ${HECT_OUTPUT_DIR}
    COMMAND HectPerformanceTests)
