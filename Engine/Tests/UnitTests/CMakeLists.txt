project(HectUnitTests CXX)

if(UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -Wall")
endif()

include_directories(
    "${PROJECT_SOURCE_DIR}/../../Source"
    "${PROJECT_SOURCE_DIR}/../../Dependencies/Catch"
    )
    
grouped_source_files(SOURCE_FILES ${PROJECT_SOURCE_DIR}/Source)
add_executable(HectUnitTests ${SOURCE_FILES})
    
target_link_libraries(HectUnitTests Hect)
    
set_target_properties(HectUnitTests PROPERTIES
    PROJECT_LABEL HectUnitTests
    LINKER_LANGUAGE CXX
    FOLDER "/Engine/Tests"
    )
