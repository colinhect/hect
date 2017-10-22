project(NanoVG C)

add_library(NanoVG STATIC
    "nanovg.c"
    )

target_include_directories(NanoVG PUBLIC ${PROJECT_SOURCE_DIR})

if(MSVC)
    target_compile_options(NanoVG PRIVATE /W0)
else()
    target_compile_options(NanoVG PRIVATE -w -fPIC)
endif()
    
set_target_properties(NanoVG PROPERTIES
    PROJECT_LABEL NanoVG
    FOLDER "/Engine/Dependencies"
    LINKER_LANGUAGE C
    )
