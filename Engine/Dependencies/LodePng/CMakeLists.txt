project(LodePng CXX)

if(WIN32)
    add_definitions("/W0")
elseif(UNIX)
    add_definitions("-w")
endif()

include_directories(
    "${PROJECT_SOURCE_DIR}"
    )

add_library(LodePng STATIC
    "lodepng.cpp"
    "lodepng.h"
    )
    
set_target_properties(LodePng PROPERTIES
    PROJECT_LABEL LodePng
    FOLDER "/Engine/Dependencies"
    LINKER_LANGUAGE CXX
    )
