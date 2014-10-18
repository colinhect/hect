include_directories(
    "${PROJECT_SOURCE_DIR}/Dependencies/Glew"
    )

add_library(Glew STATIC
    "glew.c"
    "glewinfo.c"
    "visualinfo.c"
    "GL/glew.h"
    "GL/glxew.h"
    "GL/wglew.h"
    )
    
set_target_properties(Glew PROPERTIES
    PROJECT_LABEL Glew
    FOLDER "/Dependencies"
    LINKER_LANGUAGE CXX
    )