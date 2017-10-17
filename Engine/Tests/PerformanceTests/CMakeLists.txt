project(HectPerformanceTests CXX)

if(UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -Wall -Wextra -Werror -pedantic")
endif()

add_definitions(-DCELERO_STATIC)

include_directories(
    "${PROJECT_SOURCE_DIR}/../../Source"
    "${PROJECT_SOURCE_DIR}/../../Dependencies/Celero/include"
    )

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

set(LINK_LIBRARIES Hect Celero)

if(MSVC)
    list(APPEND LINK_LIBRARIES "PowrProf.lib")
else()
    list(APPEND LINK_LIBRARIES "pthread")
endif()

target_link_libraries(HectPerformanceTests ${LINK_LIBRARIES})

set_target_properties(HectPerformanceTests PROPERTIES
    PROJECT_LABEL HectPerformanceTests
    LINKER_LANGUAGE CXX
    FOLDER "/Engine/Tests"
    )

# Copy settings
add_custom_command(TARGET HectPerformanceTests
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/Settings ${OUTPUT_DIR}/
    )

add_test(NAME HectPerformanceTests
    WORKING_DIRECTORY ${OUTPUT_DIR}
    COMMAND HectPerformanceTests)
