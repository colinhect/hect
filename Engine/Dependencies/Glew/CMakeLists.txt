project(Glew C)

add_library(Glew STATIC
    "glew.c"
    "GL/glew.h"
    "GL/glxew.h"
    "GL/wglew.h"
    )

target_include_directories(Glew PUBLIC ${PROJECT_SOURCE_DIR})

if(MSVC)
    target_compile_options(Glew PRIVATE /W0)
else()
    target_compile_options(Glew PRIVATE -w -fPIC)
endif()
    
set_target_properties(Glew PROPERTIES
    PROJECT_LABEL Glew
    FOLDER "/Engine/Dependencies"
    LINKER_LANGUAGE C
    )
