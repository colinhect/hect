project(NanoVG C)

if(WIN32)
    add_definitions("/W0")
elseif(UNIX)
    add_definitions("-w -fPIC")
endif()

include_directories(
    "${PROJECT_SOURCE_DIR}"
    )

add_library(NanoVG STATIC
    "nanovg.c"
    )
    
set_target_properties(NanoVG PROPERTIES
    PROJECT_LABEL NanoVG
    FOLDER "/Engine/Dependencies"
    LINKER_LANGUAGE C
    )
