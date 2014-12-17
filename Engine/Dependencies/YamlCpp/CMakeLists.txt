project(YamlCpp CXX)

if(WIN32)
    add_definitions("/W0 -D_SCL_SECURE_NO_WARNINGS")
elseif(UNIX)
    add_definitions("-w")
endif()

include_directories(
    "${PROJECT_SOURCE_DIR}"
    "${PROJECT_SOURCE_DIR}/../Boost/include"
    )

add_library(YamlCpp STATIC
    "binary.cpp"
    "convert.cpp"
    "directives.cpp"
    "emit.cpp"
    "emitfromevents.cpp"
    "emitter.cpp"
    "emitterstate.cpp"
    "emitterutils.cpp"
    "exp.cpp"
    "memory.cpp"
    "node.cpp"
    "nodebuilder.cpp"
    "nodeevents.cpp"
    "node_data.cpp"
    "null.cpp"
    "ostream_wrapper.cpp"
    "parse.cpp"
    "parser.cpp"
    "regex.cpp"
    "scanner.cpp"
    "scanscalar.cpp"
    "scantag.cpp"
    "scantoken.cpp"
    "simplekey.cpp"
    "singledocparser.cpp"
    "stream.cpp"
    "tag.cpp"
    "contrib/graphbuilder.cpp"
    "contrib/graphbuilderadapter.cpp"
    )
    
set_target_properties(YamlCpp PROPERTIES
    PROJECT_LABEL YamlCpp
    FOLDER "/Engine/Dependencies"
    LINKER_LANGUAGE CXX
    )
