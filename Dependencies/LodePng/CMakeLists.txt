include_directories(
    "${PROJECT_SOURCE_DIR}/Dependencies/LodePng"
    )

add_library(LodePng STATIC
    "lodepng.cpp"
    "lodepng.h"
    )
    
set_target_properties(LodePng PROPERTIES
    PROJECT_LABEL LodePng
    FOLDER "/Dependencies"
    LINKER_LANGUAGE CXX
    )