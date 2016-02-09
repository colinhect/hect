project(HectSystemTests CXX)

if(UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -Wall -Wextra -Werror -pedantic")
endif()

add_definitions(-DCELERO_STATIC)

include_directories(
    "${PROJECT_SOURCE_DIR}/../../Source"
    "${PROJECT_SOURCE_DIR}/../../Dependencies/Celero/include"
    )
    
set(SOURCE_FILES
    "Source/Context.cpp"
    "Source/Context.h"
    "Source/Main.cpp"
    "Source/SceneTests.cpp"
    )
    
source_group("Source" FILES
    "Source/Context.cpp"
    "Source/Context.h"
    "Source/Main.cpp"
    "Source/SceneTests.cpp"
    )

add_executable(HectPerformanceTests ${SOURCE_FILES})

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