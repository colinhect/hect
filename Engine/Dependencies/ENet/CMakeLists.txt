project(ENet C)

add_library(ENet STATIC
    "callbacks.c"
    "compress.c"
    "packet.c"
    "peer.c"
    "host.c"
    "list.c"
    "protocol.c"
    "unix.c"
    "win32.c"
    "enet/callbacks.h"
    "enet/enet.h"
    "enet/list.h"
    "enet/protocol.h"
    "enet/time.h"
    "enet/types.h"
    "enet/unix.h"
    "enet/utility.h"
    "enet/win32.h"
    )
    
target_include_directories(ENet PUBLIC ${PROJECT_SOURCE_DIR})

if(MSVC)
    target_compile_options(ENet PRIVATE /W0)
else()
    target_compile_options(ENet PRIVATE -w -fPIC)
endif()

set_target_properties(ENet PROPERTIES
    PROJECT_LABEL ENet
    FOLDER "/Engine/Dependencies"
    LINKER_LANGUAGE C
    )
