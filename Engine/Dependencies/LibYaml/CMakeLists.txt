project(LibYaml CXX)

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

target_include_directories(LibYaml PUBLIC ${PROJECT_SOURCE_DIR})

if(MSVC)
    target_compile_options(LibYaml PRIVATE /W0)
else()
    target_compile_options(LibYaml PRIVATE -w -fPIC)
endif()

target_compile_definitions(LibYaml PRIVATE HAVE_CONFIG_H YAML_DECLARE_STATIC)
    
set_target_properties(LibYaml PROPERTIES
    PROJECT_LABEL LibYaml
    FOLDER "/Engine/Dependencies"
    LINKER_LANGUAGE CXX
    )
