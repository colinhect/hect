project(Lua C)

if(WIN32)
    add_definitions("/W0")
elseif(UNIX)
    add_definitions("-w -fPIC")
endif()

include_directories(
    "${PROJECT_SOURCE_DIR}"
    )

add_library(Lua STATIC
    "lapi.c"
    "lauxlib.c"
    "lbaselib.c"
    "lbitlib.c"
    "lcode.c"
    "lcorolib.c"
    "lctype.c"
    "ldblib.c"
    "ldebug.c"
    "ldo.c"
    "ldump.c"
    "lfunc.c"
    "lgc.c"
    "linit.c"
    "liolib.c"
    "llex.c"
    "lmathlib.c"
    "lmem.c"
    "loadlib.c"
    "lobject.c"
    "lopcodes.c"
    "loslib.c"
    "lparser.c"
    "lstate.c"
    "lstring.c"
    "lstrlib.c"
    "ltable.c"
    "ltablib.c"
    "ltm.c"
    "lundump.c"
    "lutf8lib.c"
    "lvm.c"
    "lzio.c"
    )
    
set_target_properties(Lua PROPERTIES
    PROJECT_LABEL Lua
    FOLDER "/Engine/Dependencies"
    LINKER_LANGUAGE C
    )
