project(LibYaml CXX)

if(WIN32)
    add_definitions("/W0")
elseif(UNIX)
    add_definitions("-w")
endif()

add_definitions (-DHAVE_CONFIG_H -DYAML_DECLARE_STATIC)

include_directories(
    "${PROJECT_SOURCE_DIR}"
    )

add_library(LibYaml STATIC
    "api.c"
    "dumper.c"
    "emitter.c"
    "loader.c"
    "parser.c"
    "reader.c"
    "scanner.c"
    "writer.c"
    "yaml.h"
    "yaml_private.h"
    )
    
set_target_properties(LibYaml PROPERTIES
    PROJECT_LABEL LibYaml
    FOLDER "/Engine/Dependencies"
    LINKER_LANGUAGE CXX
    )
