project(ENet C)

if(WIN32)
    add_definitions("/W0")
elseif(UNIX)
    add_definitions("-w")
endif()

include_directories(
    "${PROJECT_SOURCE_DIR}"
    )

add_library(ENet STATIC
    "callbacks.c"
    "compress.c"
    "enet_packet.c"
    "enet_peer.c"
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
    
set_target_properties(ENet PROPERTIES
    PROJECT_LABEL ENet
    FOLDER "/Engine/Dependencies"
    LINKER_LANGUAGE C
    )
