project(LodePng CXX)

add_library(LodePng STATIC
    "lodepng.cpp"
    "lodepng.h"
    )

target_include_directories(LodePng PUBLIC ${PROJECT_SOURCE_DIR})

if(MSVC)
    target_compile_options(LodePng PRIVATE /W0)
else()
    target_compile_options(LodePng PRIVATE -w -fPIC)
endif()
    
set_target_properties(LodePng PROPERTIES
    PROJECT_LABEL LodePng
    FOLDER "/Engine/Dependencies"
    LINKER_LANGUAGE CXX
    )
